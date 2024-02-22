#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/StreamCopier.h"

#include <opencv2/imgcodecs/imgcodecs.hpp>

#include <iostream>
#include <thread>
#include <opencv2/imgproc.hpp>

#include "CAS_OCR.h"

bool decode_image(const std::string &image_data, cv::Mat &image) {
    try {
        const std::vector<uchar> data(image_data.begin(), image_data.end());
        image = cv::imdecode(data, cv::IMREAD_COLOR);
        cv::resize(image, image, cv::Size(400, 140));
    } catch (cv::Exception &ex) {
        std::cerr << "Error decoding image: " << ex.what() << std::endl;
        return false;
    }

    if (image.empty()) {
        std::cerr << "Error decoding image" << std::endl;
        return false;
    }

    return true;
}

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
    if (!decode_image(image_data, image)) {
        std::cerr << "[" << peerAddress << "] Error decoding image" << std::endl;
        return;
    }

    std::cout << "[" << peerAddress << "] " <<
            "Image Decode Success"
            << std::endl;

    // Add image processing and prediction logic here
    const auto predict_result =
            CAS_OCR::predict_validate_code(image, false);

    const std::string result =
            std::get<1>(predict_result);
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
    if (!CAS_OCR::init_model(
            "../../Checkpoint",
            "fp32")
    ) {
        std::cerr << "Error initializing model" << std::endl;
        return -1;
    }

    std::cout << "Load Model Success" << std::endl;

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
