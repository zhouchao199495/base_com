#ifndef _KAFKA_PRODUCER_H
#define _KAFKA_PRODUCER_H

#include <string>
#include <iostream>

#include "librdkafka/rdkafkacpp.h"
#include "my_log4cpp.h"


class KafkaProducer
{
public:
    KafkaProducer();
    ~KafkaProducer();
};
#endif // _KAFKA_PRODUCER_H