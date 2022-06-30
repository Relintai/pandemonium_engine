#ifndef MESSAGE_PAGE_H
#define MESSAGE_PAGE_H

#include "core/containers/vector.h"
#include "core/string.h"

#include "web/http/web_node.h"

#include "web/http/request.h"


class MessagePage : public WebNode {
    RCPP_OBJECT(MessagePage, WebNode);

public:
    void handle_request_main(Request *request);

    void _migrate(const bool clear, const bool seed_db);

    MessagePage();
    ~MessagePage();

    Vector<String> messages;
};

#endif