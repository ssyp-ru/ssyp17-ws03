#include <RealEngine/logger.h>
#include <iostream>
#include <RealEngine/graphic.h>
#include <vector>

class Log_ {
public:
    std::ofstream stream_;
    re::StopWatch clock_;
    std::vector<std::string> screen_messeg;
    std::vector<re::Log::LEVEL> screen_messeg_lvl;
    re::Log::LEVEL file_level;
    re::Log::LEVEL console_level;
    re::Log::LEVEL screen_level;
    // each msg has its index which is returned by
    unsigned int msg_count;
    unsigned int screen_msg_count;
    re::Log::LEVEL level;

    unsigned int file_msg(std::string msg, re::Log::LEVEL level) {
        if(file_level >= level) {
            stream_ << "> " << msg_count + 1 << ": [" << clock_.getTimeString() << " : " << time() << "] " << msg << std::endl;
            return msg_count;
        }
        return -1;
    }
    unsigned int msg(std::string msg, re::Log::LEVEL level) {
        if(file_level >= level) {
            stream_ << "> " << msg_count + 1 << ": [" << clock_.getTimeString() << " : " << time() << "] " << msg << std::endl;
            return msg_count;
        }
        return -1;
    }
    unsigned int console_msg(std::string msg, re::Log::LEVEL level) {
        if(console_level >= level) {
            std::cout << "> " << msg_count + 1 << ": [" << clock_.getTimeString() << " : " << time() << "] " << msg << std::endl;
            return msg_count;
        }
        return -1;
    }
    unsigned int screen_msg() {
        while (screen_messeg.size() > 10){
            screen_messeg.erase(screen_messeg.begin());
        }
        for (size_t i = 0; i < screen_messeg.size(); i++){
            if(screen_level >= screen_messeg_lvl[i]) {
                re::draw_text(0, i * 40, screen_messeg[i], re::Color(0, 0, 0));
            }
        }
        return msg_count;
        return -1;
    }
    void add_screen_msg(std::string msg, re::Log::LEVEL level) {
        screen_messeg.push_back(msg);
        screen_messeg_lvl.push_back(level);
    }

    unsigned int debug(std::string mesg) {
        file_msg(mesg, re::Log::LEVEL::DEBUG);
        console_msg(mesg, re::Log::LEVEL::DEBUG);
        add_screen_msg(mesg, re::Log::LEVEL::DEBUG);
        msg_count++;
        return msg_count;
    }

    unsigned int info(std::string mesg) {
        file_msg(mesg, re::Log::LEVEL::INFO);
        console_msg(mesg, re::Log::LEVEL::INFO);
        add_screen_msg(mesg, re::Log::LEVEL::INFO);
        msg_count++;
        return msg_count;
    }

    unsigned int trace(std::string mesg) {
        file_msg(mesg, re::Log::LEVEL::TRACE);
        console_msg(mesg, re::Log::LEVEL::TRACE);
        add_screen_msg(mesg, re::Log::LEVEL::TRACE);
        msg_count++;
        return msg_count;
    }

    std::ofstream& stream() { return stream_; }
    std::ofstream& stream_msg() {
        stream_ << "> " << ++msg_count << ": [" << clock_.getTimeString() << " : " << time() << "] ";
        return stream_;
    }
    std::ofstream& stream_msg(int& i) {
        stream_ << "> " << ++msg_count << ": [" << clock_.getTimeString() << " : " << time() << "] ";
        i = msg_count;
        return stream_;
    }
    long time() {
        return clock_.stop_watch()/1000000;
    }
    void set_file_level(re::Log::LEVEL level) {
        console_level = level;
    }
    void set_console_level(re::Log::LEVEL level) {
        file_level = level;
    }
    void set_screen_level(re::Log::LEVEL level) {
        screen_level = level;
    }
    re::Log::LEVEL get_file_level(){
        return file_level;
    }
    re::Log::LEVEL get_console_level(){
        return console_level;
    }
    re::Log::LEVEL get_screen_level(){
        return screen_level;
    }
    Log_(std::string outputFile) {
        msg_count = 0;
        level = re::Log::LEVEL::TRACE;
        clock_ = re::StopWatch();
        stream_ = std::ofstream(outputFile);
        console_msg("Log opened successfully.", re::Log::LEVEL::DEBUG);
        file_msg("Log opened successfully.", re::Log::LEVEL::DEBUG);
    }
    ~Log_() {
        console_msg("Closing the log.", re::Log::LEVEL::DEBUG);
        file_msg("Closing the log.", re::Log::LEVEL::DEBUG);
        stream_.close();
    };
};



Log_ log("log.txt");

void re::Log::set_console_level(LEVEL level) {
    log.set_console_level(level);
}
void re::Log::set_file_level(LEVEL level) {
    log.set_file_level(level);
}
void re::Log::set_screen_level(LEVEL level) {
    log.set_screen_level(level);
}
long re::Log::time() {
    return log.time();
}
re::Log::LEVEL re::Log::get_console_level(){
    return log.get_console_level();
}
re::Log::LEVEL re::Log::get_screen_level(){
    return log.get_console_level();
}
re::Log::LEVEL re::Log::get_file_level(){
    return log.get_file_level();
}

std::ofstream& re::Log::stream() {
    return log.stream();
}
uint re::Log::file_msg(std::string message, re::Log::LEVEL level) {
    return log.file_msg(message, level);
}
uint re::Log::msg(std::string message, re::Log::LEVEL level) {
    return log.msg(message, level);
}
uint re::Log::console_msg(std::string message, re::Log::LEVEL level) {
    return log.console_msg(message, level);
}
uint re::Log::screen_msg() {
    return log.screen_msg();
}
void re::Log::add_screen_msg(std::string message, re::Log::LEVEL level) {
    log.add_screen_msg(message, level);
}



uint re::Log::debug(std::string message) {
    return log.debug(message);
}
uint re::Log::info(std::string message) {
    return log.info(message);
}
uint re::Log::trace(std::string message) {
    return log.trace(message);
}
std::ofstream& re::Log::stream_msg() {
    return log.stream_msg();
}
std::ofstream& re::Log::stream_msg(int& i) {
    return log.stream_msg(i);
}