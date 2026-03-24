# Unit Test Coverage Analysis & Proposals

## Current Test Coverage Summary

### ✅ Currently Tested
1. **CrawlerTests.cpp**
   - `Crawler::run()` - Multiple requests handling (with DummyCrawler mocks)

2. **HighLevelTests.cpp**
   - `ImGuiLayer::run()` - Single loop iteration
   - Task queue response logging

---

## ❌ Uncovered Code & Test Proposals

### 1. **Crawler.cpp** - Critical Coverage Gaps

#### A. `Crawler::getLinksFromUrl()`
**Current Status**: Not tested with real scenarios

**Uncovered Branches**:
- ✗ HTTP 200 response (successful crawl)
- ✗ Non-200 status codes (error handling)
- ✗ Empty page content
- ✗ Invalid URLs

**Proposed Tests**:
```cpp
// Test 1: HTTP 200 Response - Successful Link Extraction
TEST(CrawlerTests, getLinksFromUrlReturnsLinksOnSuccess)
{
    Crawler crawler;
    // Mock WebCurl::getPage to return 200 with HTML content
    // Verify extractLinks is called and results returned
}

// Test 2: Non-200 Status Code - Error Handling
TEST(CrawlerTests, getLinksFromUrlReturnsEmptySetOnHttpError)
{
    Crawler crawler;
    // Mock WebCurl::getPage to return 404/500
    // Verify empty set is returned, no crash
}

// Test 3: Empty Response Body
TEST(CrawlerTests, getLinksFromUrlHandlesEmptyContent)
{
    Crawler crawler;
    // Mock WebCurl::getPage with empty text
    // Verify graceful handling
}
```

---

#### B. `Crawler::extractLinks()`
**Current Status**: Completely untested - Complex XML parsing logic

**Uncovered Branches**:
- ✗ Valid HTML with multiple `<a>` tags
- ✗ HTML with relative URLs (resolve to absolute)
- ✗ HTML with fragments (#anchor)
- ✗ Invalid/malformed HTML
- ✗ Empty href attributes
- ✗ xmlXPath query returning empty nodeset
- ✗ xmlBuildURI failures

**Proposed Tests**:
```cpp
// Test 1: Valid HTML with Multiple Links
TEST(CrawlerTests, extractLinksFromValidHtml)
{
    Crawler crawler;
    std::string html = R"(<html>
        <a href="https://example.com/page1">Link1</a>
        <a href="/page2">Link2</a>
        <a href="page3">Link3</a>
    </html>)";
    
    auto result = crawler.extractLinks(html, "https://example.com/");
    EXPECT_THAT(result, SizeIs(3));
    EXPECT_THAT(result, Contains("https://example.com/page1"));
    EXPECT_THAT(result, Contains("https://example.com/page2"));
}

// Test 2: Relative URL Resolution
TEST(CrawlerTests, extractLinksResolvesRelativeUrls)
{
    Crawler crawler;
    std::string html = R"(<a href="relative/path">Link</a>)";
    
    auto result = crawler.extractLinks(html, "https://example.com/base/");
    EXPECT_THAT(result, Contains("https://example.com/base/relative/path"));
}

// Test 3: Malformed HTML Graceful Handling
TEST(CrawlerTests, extractLinksHandlesMalformedHtml)
{
    Crawler crawler;
    std::string html = "<<<invalid>=== html >>>***";
    
    auto result = crawler.extractLinks(html, "https://example.com/");
    EXPECT_TRUE(result.empty());
}

// Test 4: Empty href Attributes
TEST(CrawlerTests, extractLinksSkipsEmptyHrefs)
{
    Crawler crawler;
    std::string html = R"(<html>
        <a href="">Empty</a>
        <a href="https://example.com">Valid</a>
    </html>)";
    
    auto result = crawler.extractLinks(html, "https://example.com/");
    // Should not crash, should handle gracefully
}

// Test 5: XPath Query Returns Empty Nodeset
TEST(CrawlerTests, extractLinksReturnsEmptySetWithNoLinks)
{
    Crawler crawler;
    std::string html = "<html><body><p>No links here</p></body></html>";
    
    auto result = crawler.extractLinks(html, "https://example.com/");
    EXPECT_TRUE(result.empty());
}
```

---

#### C. `Crawler::crawl()` - Threading & State Management
**Current Status**: Tested only with DummyCrawler mocks

**Uncovered Branches**:
- ✗ With actual concurrent tasks
- ✗ Empty initialRequests
- ✗ Single request (no threading benefit)
- ✗ Large number of requests (15+ threads)
- ✗ State reset between consecutive calls

**Proposed Tests**:
```cpp
// Test 1: crawl() Resets State Between Calls
TEST(CrawlerTests, crawlResetsStateBetweenCalls)
{
    Crawler crawler;
    std::set<std::string> requests1 = {"url1", "url2"};
    std::set<std::string> requests2 = {"url3"};
    
    ProtectedQueue q;
    auto result1 = crawler.crawl(requests1, q);
    auto result2 = crawler.crawl(requests2, q);
    
    // Verify no cross-contamination of state
}

// Test 2: crawl() With Empty Requests
TEST(CrawlerTests, crawlWithEmptyRequests)
{
    Crawler crawler;
    ProtectedQueue q;
    
    auto result = crawler.crawl({}, q);
    EXPECT_TRUE(result.empty());
}

// Test 3: crawl() Doesn't Alter Ready Flag
TEST(CrawlerTests, crawlDoesNotAlterReadyFlag)
{
    Crawler crawler;
    ProtectedQueue q;
    q.ready.store(false);
    
    std::set<std::string> requests = {"url1"};
    crawler.crawl(requests, q);
    
    EXPECT_FALSE(q.ready.load()); // Should remain unchanged
}
```

---

### 2. **WebCurl.cpp** - Network Layer
**Current Status**: No unit tests

**Uncovered**:
- ✗ getPage() with valid URL
- ✗ getPage() with invalid URL
- ✗ getPage() with network timeout
- ✗ getPage() response headers parsing
- ✗ getPage() SSL verification disabled (security test)

**Proposed Tests**:
```cpp
// Test 1: getPage() Returns Response
TEST(WebCurlTests, getPageReturnsResponseObject)
{
    // Mock or use test endpoint
    // Verify response object contains status_code, headers, text
}

// Test 2: getPage() Handles Network Errors
TEST(WebCurlTests, getPageHandlesNetworkError)
{
    std::string invalidUrl = "http://127.0.0.1:1/definitely/not/valid";
    // Should not crash, should return error status
}

// Test 3: SSL Verification is Disabled
TEST(WebCurlTests, getPageDisablesSSLVerification)
{
    // Verify VerifySsl(false) is used
    // This is a security review test
}
```

---

### 3. **ImGuiLogger.cpp** - UI Component Logic
**Current Status**: Only `draw()` called via mocks

**Uncovered Branches**:
- ✗ `addLog()` with various format strings
- ✗ `addSimpleLog()` with special characters
- ✗ `clear()` resets state properly
- ✗ Filter functionality (Filter.IsActive())
- ✗ Auto-scroll logic
- ✗ Copy to clipboard button
- ✗ Options popup menu

**Proposed Tests**:
```cpp
// Test 1: addLog() Formats Correctly
TEST(ImGuiLoggerTests, addLogFormatsStringCorrectly)
{
    ImGuiLogger logger;
    logger.addLog("Test: %d, %.2f, %s", 42, 3.14, "hello");
    
    // Verify buffer contains formatted string
}

// Test 2: addSimpleLog() Adds With Newline
TEST(ImGuiLoggerTests, addSimpleLogAddsNewline)
{
    ImGuiLogger logger;
    logger.addSimpleLog("line1");
    logger.addSimpleLog("line2");
    
    // Verify LineOffsets tracks both lines
    EXPECT_EQ(logger.LineOffsets.Size, 3); // Initial 0, after line1, after line2
}

// Test 3: clear() Resets Everything
TEST(ImGuiLoggerTests, clearResetsBufferAndLineOffsets)
{
    ImGuiLogger logger;
    logger.addSimpleLog("test");
    logger.clear();
    
    EXPECT_EQ(logger.Buf.size(), 0);
    EXPECT_EQ(logger.LineOffsets.Size, 1); // Only initial 0 remains
}

// Test 4: FilterText Functionality
TEST(ImGuiLoggerTests, filterTextWorks)
{
    ImGuiLogger logger;
    logger.addSimpleLog("error: something failed");
    logger.addSimpleLog("info: all good");
    
    // Set filter to "error"
    // Verify only error line passes filter
}

// Test 5: Multiple Consecutive addLog Calls
TEST(ImGuiLoggerTests, multipleAddLogCalls)
{
    ImGuiLogger logger;
    for(int i = 0; i < 100; i++) {
        logger.addLog("Line %d\n", i);
    }
    
    // Verify all lines tracked properly
    EXPECT_EQ(logger.LineOffsets.Size, 101);
}
```

---

### 4. **OpenGLModule.cpp** - Graphics/UI Backend
**Current Status**: Only mocked in tests

**Uncovered Branches**:
- ✗ `initalize()` GLFW initialization sequence
- ✗ `setWindowShouldClose()` 
- ✗ `createControlsPanel()` with state transitions
- ✗ `render()` with multi-viewport enabled
- ✗ Window creation failures
- ✗ GLEW initialization failures

**Proposed Tests**:
```cpp
// Test 1: setWindowShouldClose() Sets Flag
TEST(OpenGLModuleTests, setWindowShouldCloseSignalsWindow)
{
    OpenGLModule module;
    // Initialize module first
    // Call setWindowShouldClose()
    // Verify windowShouldClose() returns true
}

// Test 2: createControlsPanel() Button State
TEST(OpenGLModuleTests, createControlsPanelButtonDisablesWhenOpen)
{
    // Test that button is disabled when loggerWindowOpen = true
    // Test that button is enabled when loggerWindowOpen = false
}

// Test 3: Handle GLFW Init Failure
TEST(OpenGLModuleTests, handleGlfwInitFailure)
{
    // Mock glfwInit to return false
    // Verify graceful error handling
}

// Test 4: Handle Window Creation Failure
TEST(OpenGLModuleTests, handleWindowCreationFailure)
{
    // Mock glfwCreateWindow to return nullptr
    // Verify no crash, proper cleanup
}
```

---

### 5. **ImGuiLayer.cpp** - Main Orchestration Layer
**Current Status**: Basic loop tested, multiple iterations not tested

**Uncovered Branches**:
- ✗ `consumeLogs()` with empty messages
- ✗ `consumeLogs()` with multiple messages
- ✗ `shutdown()` double-shutdown protection
- ✗ Multiple loop iterations with state changes
- ✗ Opening and closing Webcrawler window mid-loop

**Proposed Tests**:
```cpp
// Test 1: consumeLogs() Processes All Messages
TEST(ImGuiLayerTests, consumeLogsProcessesAllMessages)
{
    auto logger = std::make_shared<StrictMock<ImGuiLoggerMock>>();
    
    EXPECT_CALL(*logger, addSimpleLog("msg1"));
    EXPECT_CALL(*logger, addSimpleLog("msg2"));
    EXPECT_CALL(*logger, addSimpleLog("msg3"));
    
    std::set<std::string> messages = {"msg1", "msg2", "msg3"};
    // Call consumeLogs
}

// Test 2: Double Shutdown Protection
TEST(ImGuiLayerTests, shutdownCalledTwiceDoesNothing)
{
    auto module = std::make_shared<StrictMock<OpenGLModuleMock>>();
    auto logger = std::make_shared<StrictMock<ImGuiLoggerMock>>();
    
    ImGuiLayer layer(module, logger);
    EXPECT_CALL(*module, shutDown()).Times(1);
    
    layer.shutdown();
    layer.shutdown(); // Second call should be no-op
}

// Test 3: Multiple Loop Iterations
TEST(ImGuiLayerTests, multipleLoopIterationsWithStateChanges)
{
    auto module = std::make_shared<StrictMock<OpenGLModuleMock>>();
    auto logger = std::make_shared<StrictMock<ImGuiLoggerMock>>();
    
    // Simulate: closed -> open -> receive response -> close
    EXPECT_CALL(*module, windowShouldClose())
        .WillOnce(Return(false))  // Iteration 1
        .WillOnce(Return(false))  // Iteration 2
        .WillOnce(Return(false))  // Iteration 3
        .WillOnce(Return(true));  // Exit
    
    // ... setup other expectations
}

// Test 4: Webcrawler Window State Transitions
TEST(ImGuiLayerTests, webcrawlerWindowStateTransitions)
{
    // Test: window starts closed
    // Test: clicking button opens it
    // Test: clicking X closes it
    // Test: response arrives while closed (not displayed)
}
```

---

### 6. **ProtectedQueue** - Thread-Safe Container
**Current Status**: Not directly unit tested

**Uncovered**:
- ✗ Thread safety with concurrent pushes
- ✗ Thread safety with concurrent pops
- ✗ Pop from empty queue
- ✗ Multiple requests/responses interleaved

**Proposed Tests**:
```cpp
// Test 1: Pop From Empty Queue Returns Nullopt
TEST(ProtectedQueueTests, popFromEmptyReturnsNullopt)
{
    ProtectedQueue q;
    EXPECT_FALSE(q.popRequest().has_value());
    EXPECT_FALSE(q.popResponse().has_value());
}

// Test 2: Push and Pop Maintains FIFO Order
TEST(ProtectedQueueTests, pushPopMaintainsFifoOrder)
{
    ProtectedQueue q;
    q.pushRequest("first");
    q.pushRequest("second");
    
    EXPECT_EQ(q.popRequest().value(), "first");
    EXPECT_EQ(q.popRequest().value(), "second");
}

// Test 3: Thread Safety - Concurrent Pushes
TEST(ProtectedQueueTests, threadSafeWithConcurrentPushes)
{
    ProtectedQueue q;
    std::vector<std::thread> threads;
    
    for(int i = 0; i < 10; i++) {
        threads.push_back(std::thread([&q, i] {
            q.pushRequest("request_" + std::to_string(i));
        }));
    }
    
    for(auto& t : threads) t.join();
    
    // Verify all 10 requests in queue
}

// Test 4: Ready Flag Atomicity
TEST(ProtectedQueueTests, readyFlagIsAtomic)
{
    ProtectedQueue q;
    q.ready.store(true);
    EXPECT_TRUE(q.ready.load());
}
```

---

## Test Implementation Priority

### Phase 1 - High Priority (Core Logic)
1. `Crawler::extractLinks()` - Most complex untested logic
2. `Crawler::getLinksFromUrl()` - Error handling paths
3. `ProtectedQueue` basics - Foundation for integration

### Phase 2 - Medium Priority (State Management)
4. `ImGuiLogger::addLog()` format testing
5. `ImGuiLayer::consumeLogs()` 
6. `ImGuiLayer::shutdown()` double-call protection

### Phase 3 - Lower Priority (Edge Cases)
7. `OpenGLModule` error handling
8. `WebCurl` network scenarios
9. `Crawler::crawl()` threading edge cases

---

## Summary Statistics

| Component | Lines | Tested | Untested | Coverage |
|-----------|-------|--------|----------|----------|
| Crawler.cpp | ~160 | ~30 | ~130 | ~19% |
| WebCurl.cpp | ~10 | ~0 | ~10 | ~0% |
| ImGuiLogger.cpp | ~130 | ~5 | ~125 | ~4% |
| OpenGLModule.cpp | ~220 | ~50 | ~170 | ~23% |
| ImGuiLayer.cpp | ~70 | ~40 | ~30 | ~57% |
| **TOTAL** | **~590** | **~125** | **~465** | **~21%** |

**Recommendation**: Focus on Phase 1 tests first - they cover the most critical untested logic paths.
