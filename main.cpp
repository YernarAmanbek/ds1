// PROVIDED: command parsing, validation and JSON protocol. Do not edit.
#include "BrowserHistory.h"
#include "ImageQueue.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

static std::string quote(const std::string& s) {
    std::string result = "\"";
    const char* hex = "0123456789abcdef";
    for (unsigned char c : s) {
        if (c == '"' || c == '\\') { result += '\\'; result += static_cast<char>(c); }
        else if (c < 32) { result += "\\u00"; result += hex[c >> 4]; result += hex[c & 15]; }
        else result += static_cast<char>(c);
    }
    return result + "\"";
}
static bool validUrl(const std::string& s) {
    if (s.empty() || s.size() > 2048) return false;
    for (unsigned char c : s) if (c <= 32 || c == 127) return false;
    return s == "lab://home" || s == "lab://arrays" || s == "lab://stacks" ||
           s == "lab://queues" || s.rfind("https://", 0) == 0 || s.rfind("http://", 0) == 0;
}
static void state(const BrowserHistory& h, bool ok, const std::string& message) {
    // Build a complete line before writing, even if an unfinished method throws.
    std::ostringstream out;
    out << "{\"ok\":" << (ok ? "true" : "false") << ",\"message\":" << quote(message)
        << ",\"current\":" << quote(h.current()) << ",\"history\":[";
    const auto& stack = h.previous();
    for (std::size_t i = 0; i < stack.size(); ++i) {
        if (i != 0) out << ',';
        out << quote(stack.itemAt(i));
    }
    out << "],\"size\":" << stack.size() << ",\"forward\":[";
    const auto& next = h.next();
    for (std::size_t i = 0; i < next.size(); ++i) {
        if (i != 0) out << ',';
        out << quote(next.itemAt(i));
    }
    out << "],\"forwardSize\":" << next.size() << '}';
    std::cout << out.str() << std::endl;
}
static void queueState(const ImageQueue& q, bool ok, const std::string& message,
                       const std::string& item = "") {
    std::ostringstream out;
    out << "{\"ok\":" << (ok ? "true" : "false") << ",\"message\":" << quote(message)
        << ",\"kind\":\"queue\",\"item\":" << quote(item) << ",\"queue\":[";
    for (std::size_t i = 0; i < q.size(); ++i) {
        if (i) out << ',';
        out << quote(q.itemAt(i));
    }
    out << "],\"size\":" << q.size() << '}';
    std::cout << out.str() << std::endl;
}
int main(int argc, char* argv[]) {
    std::size_t capacity = 64, queueCapacity = 256;
    try {
        if (argc > 3) throw std::invalid_argument("arguments");
        for (int i = 1; i < argc; ++i) {
            const std::string arg(argv[i]);
            if (arg.empty() || arg.find_first_not_of("0123456789") != std::string::npos)
                throw std::invalid_argument("capacity");
            const unsigned long n = std::stoul(arg);
            if (n < 1 || n > 4096) throw std::invalid_argument("capacity");
            if (i == 1) capacity = static_cast<std::size_t>(n);
            else queueCapacity = static_cast<std::size_t>(n);
        }
    } catch (const std::exception&) {
        std::cerr << "Usage: browser_engine [stack-capacity 1..4096] [queue-capacity 1..4096]\n"; return 2;
    }
    BrowserHistory history(capacity);
    ImageQueue images(queueCapacity);
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        try {
            if (line == "QUIT") { std::cout << "{\"ok\":true,\"message\":\"BYE\"}" << std::endl; break; }
            if (line == "QSTATE") queueState(images, true, "QSTATE");
            else if (line == "QCLEAR") { images.clear(); queueState(images, true, "QCLEAR"); }
            else if (line == "QTAKE") {
                if (images.empty()) queueState(images, false, "EMPTY");
                else { const std::string item = images.front(); images.dequeue(); queueState(images, true, "QTAKE", item); }
            } else if (line.rfind("QADD ", 0) == 0 && validUrl(line.substr(5)) &&
                       (line.substr(5).rfind("http://", 0) == 0 || line.substr(5).rfind("https://", 0) == 0)) {
                const bool ok = images.enqueue(line.substr(5)); queueState(images, ok, ok ? "QADD" : "FULL");
            } else if (line == "STATE") state(history, true, "STATE");
            else if (line == "RESET") { history.reset(); state(history, true, "RESET"); }
            else if (line == "BACK") {
                const bool ok = history.back(); state(history, ok, ok ? "BACK" : (history.previous().empty() ? "EMPTY" : "FULL"));
            } else if (line == "FORWARD") {
                const bool ok = history.forward(); state(history, ok, ok ? "FORWARD" : (history.next().empty() ? "EMPTY" : "FULL"));
            } else if (line.rfind("VISIT ", 0) == 0 && validUrl(line.substr(6))) {
                const bool ok = history.visit(line.substr(6)); state(history, ok, ok ? "VISIT" : "FULL");
            } else state(history, false, "INVALID_COMMAND");
        } catch (const std::exception& e) {
            std::cout << "{\"ok\":false,\"message\":" << quote(e.what()) << '}' << std::endl;
        }
    }
}
