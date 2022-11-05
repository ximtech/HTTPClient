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


static HTTPResponse sendHttpGETCallback(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", client->url.protocol);
    assert_string_equal("httpbin.org", client->url.host);
    assert_uint32(147, ==, dataLength);

    assert_string_equal("GET /api?param2=value2&param1=value1&key1=val1&key2=val2 HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        client->requestBuffer);

    assert_true(isHashMapNotEmpty(client->queryParameters));
    assert_uint32(getHashMapSize(client->queryParameters), ==, 2);
    assert_string_equal(hashMapGet(client->queryParameters, "param1"), "value1");
    assert_string_equal(hashMapGet(client->queryParameters, "param2"), "value2");

    assert_true(isHashMapNotEmpty(client->headers));
    assert_uint32(getHashMapSize(client->headers), ==, 4);
    assert_string_equal(hashMapGet(client->headers, "Host"), "httpbin.org");
    assert_string_equal(hashMapGet(client->headers, "Connection"), "close");
    assert_string_equal(hashMapGet(client->headers, "User-Agent"), "FakeAgent");
    assert_string_equal(hashMapGet(client->headers, "Custom"), "header");

    assert_true(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpPOSTCallback(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", client->url.protocol);
    assert_string_equal("httpbin.org", client->url.host);
    assert_uint32(147, ==, dataLength);

    assert_string_equal("POST /api HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "Content-Length: 27\r\n"    // content length header should be present
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n"
                        "param2=value2&param1=value1\r\n\r\n",
                        client->requestBuffer);

    assert_true(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpPOSTJsonCallback(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", client->url.protocol);
    assert_string_equal("httpbin.org", client->url.host);
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
                        client->requestBuffer);

    assert_true(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpPUTCallback(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", client->url.protocol);
    assert_string_equal("httpbin.org", client->url.host);
    assert_uint32(99, ==, dataLength);

    assert_string_equal("PUT /api HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        client->requestBuffer);

    assert_false(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpDELETECallback(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", client->url.protocol);
    assert_string_equal("httpbin.org", client->url.host);
    assert_uint32(109, ==, dataLength);

    assert_string_equal("DELETE /api/1/test HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        client->requestBuffer);

    assert_false(isBlockingExecute);
    return httpResponse;
}

static HTTPResponse sendHttpHEADCallback(HTTPClient *client, uint32_t dataLength, bool isBlockingExecute) {
    HTTPResponse httpResponse = {HTTP_OK, NULL, NULL};
    assert_string_equal("http", client->url.protocol);
    assert_string_equal("httpbin.org", client->url.host);
    assert_uint32(107, ==, dataLength);

    assert_string_equal("HEAD /api/1/test HTTP/1.1\r\n"
                        "Host: httpbin.org\r\n"
                        "Connection: close\r\n"
                        "User-Agent: FakeAgent\r\n"
                        "Custom: header\r\n\r\n",
                        client->requestBuffer);

    assert_false(isBlockingExecute);
    return httpResponse;
}


static MunitResult httpClientGETTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpGETCallback);

    HTTPResponse response = client.GET(&client, "http://httpbin.org/api?key1=val1&key2=val2")
            .bindParam(&client, "param1", "value1")
            .bindParam(&client, "param2", "value2")
            .addHeader(&client, CONNECTION, "close")
            .addHeader(&client, USER_AGENT, "FakeAgent")
            .addCustomHeader(&client, "Custom", "header")
            .execute(&client);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient(&client);
    return MUNIT_OK;
}

static MunitResult httpClientPOSTTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpPOSTCallback);

    HTTPResponse response = client.POST(&client, "http://httpbin.org/api")
            .bindParam(&client, "param1", "value1")
            .bindParam(&client, "param2", "value2")
            .addHeader(&client, CONNECTION, "close")
            .addHeader(&client, USER_AGENT, "FakeAgent")
            .addCustomHeader(&client, "Custom", "header")
            .execute(&client);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient(&client);
    return MUNIT_OK;
}

static MunitResult httpClientPOSTJsonTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpPOSTJsonCallback);

    HTTPResponse response = client.POST(&client, "http://httpbin.org/api")
            .bindJson(&client, jsonExample)
            .addHeader(&client, CONNECTION, "close")
            .addHeader(&client, USER_AGENT, "FakeAgent")
            .addCustomHeader(&client, "Custom", "header")
            .execute(&client);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient(&client);
    return MUNIT_OK;
}

static MunitResult httpClientPUTTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpPUTCallback);

    HTTPResponse response = client.PUT(&client, "http://httpbin.org/api")
            .addHeader(&client, CONNECTION, "close")
            .addHeader(&client, USER_AGENT, "FakeAgent")
            .addCustomHeader(&client, "Custom", "header")
            .executeNonBlock(&client);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient(&client);
    return MUNIT_OK;
}

static MunitResult httpClientDELETETest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpDELETECallback);

    HTTPResponse response = client.DELETE(&client, "http://httpbin.org/api/1/test")
            .addHeader(&client, CONNECTION, "close")
            .addHeader(&client, USER_AGENT, "FakeAgent")
            .addCustomHeader(&client, "Custom", "header")
            .executeNonBlock(&client);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient(&client);
    return MUNIT_OK;
}

static MunitResult httpClientHEADTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpHEADCallback);

    HTTPResponse response = client.HEAD(&client, "http://httpbin.org/api/1/test")
            .addHeader(&client, CONNECTION, "close")
            .addHeader(&client, USER_AGENT, "FakeAgent")
            .addCustomHeader(&client, "Custom", "header")
            .executeNonBlock(&client);

    assert_int(HTTP_OK, ==, response.statusCode);

    deleteHttpClient(&client);
    return MUNIT_OK;
}

static MunitResult httpClientInvalidDataTest(const MunitParameter params[], void *data) {
    char buffer[BUFFER_SIZE] = {0};
    HTTPClient client = {0};
    initHTTPClient(&client, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, sendHttpGETCallback);

    HTTPResponse response = client.GET(&client, "test").execute(&client);    // invalid URL
    assert_int(HTTP_NO_STATUS, ==, response.statusCode);

    initHTTPClient(NULL, buffer, BUFFER_SIZE);
    registerHttpCallback(&client, NULL);
    response = client.GET(&client, "http://httpbin.org/api").execute(NULL);    // NULL pointer
    assert_int(HTTP_NO_STATUS, ==, response.statusCode);

    HTTPClient invalidClient = {0};
    initHTTPClient(&invalidClient, buffer, 0);
    registerHttpCallback(&invalidClient, sendHttpGETCallback);
    response = invalidClient.GET(&invalidClient, "http://httpbin.org/api").execute(&invalidClient);    // wrong buffer size
    assert_int(HTTP_NO_STATUS, ==, response.statusCode);

    deleteHttpClient(&client);
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