# HTTPClient Client

[![tests](https://github.com/ximtech/HTTPClient/actions/workflows/cmake-ci.yml/badge.svg)](https://github.com/ximtech/HTTPClient/actions/workflows/cmake-ci.yml)
[![codecov](https://codecov.io/gh/ximtech/HTTPClient/branch/master/graph/badge.svg?token=NRezIEBnBn)](https://codecov.io/gh/ximtech/HTTPClient)

Standalone library for formatting HTTPClient requests.

### Features 

- Pluggable HTTPClient transport abstraction
- Easily to make HTTPClient GET, POST, PUT, DELETE and HEAD requests to a web server
- Single callback method for low level library interaction
- Provides auto Content-Length header with length for POST requests 
- Provides functional style requests

### Dependencies

- [HashMap library](https://github.com/ximtech/HashMap)
- [Ethernet library](https://github.com/ximtech/Ethernet)
- [URL Parser library](https://github.com/ximtech/URLParser)

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
#include "HTTPClient.h"

// Create callback method at implementor side
HTTPResponse sendHttpRequestCallback(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute) {
    printf("Protocol: %s\n", client->url.protocol);
    printf("Host: %s\n", client->url.host);
    printf("Data length: %d\n", dataLength);

    // Formatted GET request
    assert_string_equal(
        "GET /api?param2=value2&param1=value1 HTTP/1.1\r\n"
        "Host: httpbin.org\r\n"
        "Connection: close\r\n"
        "User-Agent: FakeAgent\r\n"
        "Custom: header\r\n\r\n",
        client->requestBuffer);

    // Specific hardware implementation that send request to server
    HTTPResponse httpResponse = doRequestToServer(requestBuffer, dataLength);
    return httpResponse;
}

// User side, send request
void sendGetRequest() {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpGETCallback);

HTTPResponse response = client.GET(&client, "http://httpbin.org/api")
        .bindParam(&client, "param1", "value1")
        .bindParam(&client, "param2", "value2")
        .addHeader(&client, CONNECTION, "close")
        .addHeader(&client, USER_AGENT, "FakeAgent")
        .addCustomHeader(&client, "Custom", "header")
        .execute(&client);
    
    assert_int(HTTP_OK, ==, getResp.statusCode);
}
```
