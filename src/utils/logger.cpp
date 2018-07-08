#include <RealEngine/logger.h>

class Log_ {
public:
    std::ofstream stream_;
    re::StopWatch clock_;

    // each msg has its index which is returned by
    unsigned int msg_count;
    re::Log::LEVEL level;

    unsigned int msg(std::string msg, re::Log::LEVEL level) {
        if(this->level >= level) {
            stream_ << "> " << ++msg_count << ": [" << clock_.getTimeString() << " : " << time() << "] " << msg << std::endl;
            return msg_count;
        }
        return -1;
    }
        
    void TRACE (std::string mesg){
        msg(mesg, re::Log::LEVEL::TRACE);
    }
    
    void DEBUG (std::string mesg){
        msg(mesg, re::Log::LEVEL::DEBUG);
    }

    void INFO (std::string mesg){
        msg(mesg, re::Log::LEVEL::INFO);
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
    void log_level(re::Log::LEVEL) {
        this->level = level;
    }
    Log_(std::string outputFile) {
        msg_count = 0;
        level = re::Log::LEVEL::TRACE;
        clock_ = re::StopWatch();
        stream_ = std::ofstream(outputFile);
        msg("Log opened successfully.", re::Log::LEVEL::DEBUG);
    }
    ~Log_() {
        msg("Closing the log.", re::Log::LEVEL::DEBUG);
        stream_.close();
    };
};



Log_ log("log.txt");

void re::Log::log_level(LEVEL level) {
    log.log_level(level);
}
long re::Log::time() {
    return log.time();
}
std::ofstream& re::Log::stream() {
    return log.stream();
}
uint re::Log::msg(std::string message, re::Log::LEVEL level) {
    return log.msg(message, level);
}
std::ofstream& re::Log::stream_msg() {
    return log.stream_msg();
}
std::ofstream& re::Log::stream_msg(int& i) {
    return log.stream_msg(i);
}