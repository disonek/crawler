#include <spdlog/spdlog.h>

#include <string>

#include "Crawler.hpp"
#include "Timer.hpp"
#include "Trace.hpp"
#include "cxxopts.hpp"

int main(int argc, char* argv[])
{
    Timer timer(__func__);
    utils::Instrumentor::get().beginSession("Session Name");
    uint8_t numThreads = 15;
    std::string url;
    bool needHelp = false;
    bool printVersion = false;
    std::string link;

    cxxopts::Options options(argv[0]);
    options.add_options()("h, help", "Get help message", cxxopts::value<bool>(needHelp))(
        "t, threads", "Set amount of threads (default 4)", cxxopts::value<uint8_t>(numThreads))(
        "a, link", "Set the link to begin downloading from", cxxopts::value<std::string>(link));
    options.parse(argc, argv);

    if(needHelp)
    {
        spdlog::info("{}", options.help().c_str());
        return 0;
    }
    if(!link.empty())
    {
        url = link;
    }
    else
    {
        url = "https://www.google.com/doodles";
    }

    auto crawler = webcrawler::Crawler{numThreads};

    spdlog::warn("Number of threads {}", numThreads);
    spdlog::info("Initial request");
    std::set<std::string> initialRequests = crawler.getLinksFromUrl(url);

    spdlog::info("Crawl");
    crawler.crawl(initialRequests);

    utils::Instrumentor::get().endSession();
    return 0;
}

// #include "imgui.h"
// #include "imgui-SFML.h"

// #include <SFML/Graphics/RenderWindow.hpp>
// #include <SFML/System/Clock.hpp>
// #include <SFML/Window/Event.hpp>
// #include <SFML/Graphics/CircleShape.hpp>

// int main()
// {
//     sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
//     window.setFramerateLimit(60);
//     ImGui::SFML::Init(window);

//     sf::CircleShape shape(100.f);
//     shape.setFillColor(sf::Color::Green);

//     sf::Clock deltaClock;
//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             ImGui::SFML::ProcessEvent(event);

//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }
//         }

//         ImGui::SFML::Update(window, deltaClock.restart());

//         ImGui::ShowDemoWindow();

//         ImGui::Begin("Hello, world!");
//         ImGui::Button("Look at this pretty button");
//         ImGui::End();

//         window.clear();
//         window.draw(shape);
//         ImGui::SFML::Render(window);
//         window.display();
//     }

//     ImGui::SFML::Shutdown();

//     return 0;
// }
