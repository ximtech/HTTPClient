#pragma once

#include "BaseTestTemplate.h"
#include "HTTPClient.h"

#define BUFFER_SIZE 1000

static const char *jsonExample =
        "{\"widget\": {\n"
        "    \"debug\": \"on\",\n"
        "    \"window\": {\n"
        "        \"title\": \"Sample Konfabulator Widget\",\n"
        "        \"name\": \"main_window\",\n"
        "        \"width\": 500,\n"
        "        \"height\": 500\n"
        "    },\n"
        "    \"image\": { \n"
        "        \"src\": \"Images/Sun.png\",\n"
        "        \"name\": \"sun1\",\n"
        "        \"hOffset\": 250,\n"
        "        \"vOffset\": 250,\n"
        "        \"alignment\": \"center\"\n"
        "    },\n"
        "    \"text\": {\n"
        "        \"data\": \"Click Here\",\n"
        "        \"size\": 36,\n"
        "        \"style\": \"bold\",\n"
        "        \"name\": \"text1\",\n"
        "        \"hOffset\": 250,\n"
        "        \"vOffset\": 100,\n"
        "        \"alignment\": \"center\",\n"
        "        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"\n"
        "    }\n"
        "}}";


static HTTPResponse sendHttpGETCallback(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", url->protocol);
    assert_string_equal("httpbin.org", url->host);
    assert_uint32(127, ==, dataLength);

    assert_string_equal("GET /api?param2=value2&param1=value1 HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        requestBuffer);

    assert_true(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpPOSTCallback(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", url->protocol);
    assert_string_equal("httpbin.org", url->host);
    assert_uint32(147, ==, dataLength);

    assert_string_equal("POST /api HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "Content-Length: 27\r\n"    // content length header should be present
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n"
                        "param2=value2&param1=value1\r\n\r\n",
                        requestBuffer);

    assert_true(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpPOSTJsonCallback(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", url->protocol);
    assert_string_equal("httpbin.org", url->host);
    assert_uint32(723, ==, dataLength);

    assert_string_equal("POST /api HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "Content-Length: 602\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n"
                        "{\"widget\": {\n    "
                        "\"debug\": \"on\",\n    "
                        "\"window\": {\n        "
                        "\"title\": \"Sample Konfabulator Widget\",\n        "
                        "\"name\": \"main_window\",\n        "
                        "\"width\": 500,\n        "
                        "\"height\": 500\n    },\n    "
                        "\"image\": { \n        "
                        "\"src\": \"Images/Sun.png\",\n        "
                        "\"name\": \"sun1\",\n        "
                        "\"hOffset\": 250,\n        "
                        "\"vOffset\": 250,\n        "
                        "\"alignment\": \"center\"\n    "
                        "},\n    "
                        "\"text\": {\n        "
                        "\"data\": \"Click Here\",\n        "
                        "\"size\": 36,\n        "
                        "\"style\": \"bold\",\n        "
                        "\"name\": \"text1\",\n        "
                        "\"hOffset\": 250,\n        "
                        "\"vOffset\": 100,\n        "
                        "\"alignment\": \"center\",\n        "
                        "\"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"\n    "
                        "}\n}}\r\n\r\n",
                        requestBuffer);

    assert_true(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpPUTCallback(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", url->protocol);
    assert_string_equal("httpbin.org", url->host);
    assert_uint32(99, ==, dataLength);

    assert_string_equal("PUT /api HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        requestBuffer);

    assert_true(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpDELETECallback(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", url->protocol);
    assert_string_equal("httpbin.org", url->host);
    assert_uint32(109, ==, dataLength);

    assert_string_equal("DELETE /api/1/test HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        requestBuffer);

    assert_false(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpHEADCallback(URLParser *url, const char *requestBuffer, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", url->protocol);
    assert_string_equal("httpbin.org", url->host);
    assert_uint32(107, ==, dataLength);

    assert_string_equal("HEAD /api/1/test HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        requestBuffer);

    assert_false(isBlockingExecute);
    return httpResponse;
}


static MunitResult httpClientGETTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpGETCallback);

    HTTPResponse response = http.GET("http://httpbin.org/api")
            .bindParam("param1", "value1")
            .bindParam("param2", "value2")
            .addHeader(CONNECTION, "close")
            .addHeader(USER_AGENT, "FakeAgent")
            .addCustomHeader("Custom", "header")
            .execute(&http);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient();
    return MUNIT_OK;
}

static MunitResult httpClientPOSTTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpPOSTCallback);

    HTTPResponse response = http.POST("http://httpbin.org/api")
            .bindParam("param1", "value1")
            .bindParam("param2", "value2")
            .addHeader(CONNECTION, "close")
            .addHeader(USER_AGENT, "FakeAgent")
            .addCustomHeader("Custom", "header")
            .execute(&http);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient();
    return MUNIT_OK;
}

static MunitResult httpClientPOSTJsonTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpPOSTJsonCallback);

    HTTPResponse response = http.POST("http://httpbin.org/api")
            .bindJson(jsonExample)
            .addHeader(CONNECTION, "close")
            .addHeader(USER_AGENT, "FakeAgent")
            .addCustomHeader("Custom", "header")
            .execute(&http);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient();
    return MUNIT_OK;
}

static MunitResult httpClientPUTTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpPUTCallback);

    HTTPResponse response = http.PUT("http://httpbin.org/api")
            .addHeader(CONNECTION, "close")
            .addHeader(USER_AGENT, "FakeAgent")
            .addCustomHeader("Custom", "header")
            .execute(&http);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient();
    return MUNIT_OK;
}

static MunitResult httpClientDELETETest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpDELETECallback);

    HTTPResponse response = http.DELETE("http://httpbin.org/api/1/test")
            .addHeader(CONNECTION, "close")
            .addHeader(USER_AGENT, "FakeAgent")
            .addCustomHeader("Custom", "header")
            .executeNonBlock(&http);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient();
    return MUNIT_OK;
}

static MunitResult httpClientHEADTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpHEADCallback);

    HTTPResponse response = http.HEAD("http://httpbin.org/api/1/test")
            .addHeader(CONNECTION, "close")
            .addHeader(USER_AGENT, "FakeAgent")
            .addCustomHeader("Custom", "header")
            .executeNonBlock(&http);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient();
    return MUNIT_OK;
}

static MunitResult httpClientInvalidDataTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTP http = initHTTPInstance(buffer, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpGETCallback);

    HTTPResponse response = http.GET("test").execute(&http);    // invalid URL
    assert_int(HTTP_NO_STATUS, ==, response.statusCode);

    http = initHTTPInstance(NULL, BUFFER_SIZE);
    registerHttpCallback(&http, sendHttpGETCallback);
    response = http.GET("http://httpbin.org/api").execute(&http);    // NULL pointer
    assert_int(HTTP_NO_STATUS, ==, response.statusCode);

    http = initHTTPInstance(buffer, 0);
    registerHttpCallback(&http, sendHttpGETCallback);
    response = http.GET("http://httpbin.org/api").execute(&http);    // wrong buffer size
    assert_int(HTTP_NO_STATUS, ==, response.statusCode);

    deleteHttpClient();
    return MUNIT_OK;
}

static MunitTest httpClientTests[] = {
        {.name = "Test OK - should correctly format GET request", .test = httpClientGETTest},
        {.name = "Test OK - should correctly format POST request", .test = httpClientPOSTTest},
        {.name = "Test OK - should correctly format POST JSON request", .test = httpClientPOSTJsonTest},
        {.name = "Test OK - should correctly format PUT request", .test = httpClientPUTTest},
        {.name = "Test OK - should correctly format DELETE request", .test = httpClientDELETETest},
        {.name = "Test OK - should correctly format HEAD request", .test = httpClientHEADTest},
        {.name = "Test FAIL - should correctly handle invalid requests", .test = httpClientInvalidDataTest},

        END_OF_TESTS
};

static const MunitSuite httpClientTestSuite = {
        .prefix = "HTTP client: ",
        .tests = httpClientTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};