# HTTP Client

Standalone library for formatting HTTP requests.

### Features 

- Pluggable HTTP transport abstraction
- Easily to make HTTP GET, POST, PUT, DELETE and HEAD requests to a web server
- Single callback method for low level library interaction
- Provides auto Content-Length header with length for POST requests 
- Provides functional style requests

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME HTTPClient
        GITHUB_REPOSITORY ximtech/HTTPClient
        GIT_TAG origin/main)

target_link_libraries(${PROJECT_NAME} HTTPClient)
```

```cmake
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
# For Clion STM32 plugin generated Cmake use 
target_link_libraries(${PROJECT_NAME}.elf HTTPClient)
```

### Usage
```c
// Create callback method at implementor side
HTTPResponse sendHttpRequestCallback(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute) {
    printf("Protocol: %s\n", url->protocol);
    printf("Host: %s\n", url->host);
    printf("Data length: %d\n", dataLength);

    // Formatted GET request
    assert_string_equal(
        "GET /api?param2=value2&param1=value1 HTTP/1.1\r\n"
        "Host: httpbin.org\r\n"
        "Connection: close\r\n"
        "User-Agent: FakeAgent\r\n"
        "Custom: header\r\n\r\n",
        requestBuffer);

    // Specific hardware implementation that send request
    HTTPResponse httpResponse = doRequestToServer(requestBuffer, dataLength);
    return httpResponse;
}

// User side, send request
void sendGetRequest() {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpRequestCallback);   // register callback function
    
    HTTPResponse getResp = http.GET("http://httpbin.org/api")  // create request
            .bindParam("param1", "value1")
            .bindParam("param2", "value2")
            .addHeader(CONNECTION, "close")
            .addHeader(USER_AGENT, "FakeAgent")
            .addCustomHeader("Custom", "header")
            .execute(&http);
    
    assert_int(HTTP_OK, ==, getResp.statusCode);
}
```