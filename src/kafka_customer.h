//#ifndef __KAFKA_CUSTOMER_H
//#define __KAFKA_CUSTOMER_H
//
//#include <string>
//#include <iostream>
//
//#include "librdkafka/rdkafkacpp.h"
//#include "my_log4cpp.h"
//
//class KafkaCustomer
//{
//public:
//    KafkaCustomer(const std::string& brokers, const std::string& topics, std::string groupid, int64_t offset=-1);
//    virtual ~KafkaCustomer();
//
//public:
//    bool connectKafka();
//    bool consume(int timeout_ms);
//    void finalize();
//    void consumer(RdKafka::Message *msg, void *opt);
//
//private:
//    std::string brokers;
//    std::string topics;
//    std::string groupid;
//
//    int64_t last_offset_ = 0;
//    RdKafka::Consumer *kafka_consumer_ = NULL; 
//    RdKafka::Topic *topic = NULL;
//    int64_t  offset_  = RdKafka::Topic::OFFSET_BEGINNING;
//    int32_t  partition_ = 0;
//};
//
//#endif //__KAFKA_CUSTOMER_H
