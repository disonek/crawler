#include <gtest/gtest.h>
using namespace testing;
// to clean
#include <Crawler2.hpp>

namespace chrono = std::chrono; // from <chrono>
namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

TEST(crawler2, main)
{
    // Check command line arguments.

    auto const threads = std::max<int>(1, 12);

    // The io_context is used to aggregate the statistics
    net::io_context ioc;

    // The report holds the aggregated statistics
    crawl_report report{ioc};

    timer t;

    // Create and launch the worker threads.
    std::vector<std::thread> workers;
    workers.reserve(threads + 1);
    for(int i = 0; i < threads; ++i)
    {
        // Each worker will eventually add some data to the aggregated
        // report. Outstanding work is tracked in each worker to
        // represent the forthcoming delivery of this data by that
        // worker.
        auto reporting_work = net::require(ioc.get_executor(), net::execution::outstanding_work.tracked);

        workers.emplace_back([&report, reporting_work] {
            // We use a separate io_context for each worker because
            // the asio resolver simulates asynchronous operation using
            // a dedicated worker thread per io_context, and we want to
            // do a lot of name resolutions in parallel.
            net::io_context ioc;
            std::make_shared<worker>(report, ioc)->run();
            ioc.run();
        });
    }

    // Add another thread to run the main io_context which
    // is used to aggregate the statistics
    workers.emplace_back([&ioc] { ioc.run(); });

    // Now block until all threads exit
    for(std::size_t i = 0; i < workers.size(); ++i)
        workers[i].join();

    std::cout << "Elapsed time:    " << chrono::duration_cast<chrono::seconds>(t.elapsed()).count() << " seconds\n";
    std::cout << report;

    // return EXIT_SUCCESS;
}