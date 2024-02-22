#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"

#include <opencv2/imgcodecs/imgcodecs.hpp>

#include <iostream>
#include <thread>

#include "CAS_OCR.h"

void handle_client(Poco::Net::StreamSocket client, const std::string &peerAddress) {
    const std::string end_marker = "<END>";
    std::string image_data;
    bool receive_error = false;

    while (true) {
        try {
            char buffer[1024];
            const int bytes_received =
                    client.receiveBytes(buffer, sizeof(buffer));
            if (bytes_received <= 0) {
                break;
            }
            image_data.append(buffer, bytes_received);
        } catch (Poco::Exception &) {
            receive_error = true;
            break;
        }

        if (
            image_data.length() >= end_marker.length() &&
            image_data.substr(
                image_data.length() - end_marker.length()
            ) == end_marker
        ) {
            image_data.erase(image_data.length() - end_marker.length());
            break;
        }
    }

    if (receive_error) {
        std::cerr << "[" << peerAddress << "] Error receiving data!" << std::endl;
        return;
    }

    std::cout << "[" << peerAddress << "] Received image data" << std::endl;

    cv::Mat image;
    try {
        std::vector<uchar> data(image_data.begin(), image_data.end());
        image = cv::imdecode(data, cv::IMREAD_COLOR);
    } catch (cv::Exception &ex) {
        std::cerr << "[" << peerAddress << "] Error decoding image: " <<
                ex.what()
                << std::endl;
        return;
    }

    if (image.empty()) {
        std::cerr << "[" << peerAddress << "] Error decoding image" << std::endl;
        return;
    }

    std::cout << "[" << peerAddress << "] " <<
            "Image Decode Success"
            << std::endl;

    // Add image processing and prediction logic here

    std::string result = "Prediction result";
    try {
        client.sendBytes(
            result.c_str(),
            static_cast<int>(result.length())
        );
    } catch (Poco::Exception &ex) {
        std::cerr << "[" << peerAddress << "] Error sending result: " << ex.displayText() <<
                std::endl;
    }

    client.close();
    std::cout << "[" << peerAddress << "] Connection closed" << std::endl;
}

[[noreturn]] int main() {
    Poco::Net::ServerSocket srv(21601);

    std::cout << "Server started, listening on port 21601" << std::endl;

    while (true) {
        Poco::Net::StreamSocket client =
                srv.acceptConnection();

        const std::string peerAddress =
                client.peerAddress().toString();
        std::cout << "Connection from: " << peerAddress << std::endl;

        // Create a new thread to handle the client connection
        std::thread client_thread(handle_client, std::move(client), peerAddress);
        // Detach the thread to execute independently in the background
        client_thread.detach();
    }

    return 0;
}
