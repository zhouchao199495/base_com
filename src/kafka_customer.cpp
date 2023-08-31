//#include "kafka_customer.h"
//
//bool run_ = true;
//static void sigterm(int sig)
//{
//    run_ = false;
//}
//
//KafkaCustomer::KafkaCustomer(const std::string &brokers, const std::string &topics, std::string groupid, int64_t offset)
//    : brokers(brokers), topics(topics), groupid(groupid), offset_(offset)
//{
//}
//
//KafkaCustomer::~KafkaCustomer()
//{
//}
//
//bool KafkaCustomer::connectKafka()
//{
//    RdKafka::Conf *conf = NULL;
//    conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
//    if (!conf)
//    {
//        LOGERROR("RdKafka create global conf failed");
//        return false;
//    }
//
//    std::string errstr;
//    /*设置broker list*/
//    // 设置kafka的地址
//    if (conf->set("bootstrap.servers", brokers, errstr) != RdKafka::Conf::CONF_OK)
//    {
//        LOGERROR("RdKafka conf set brokerlist failed:" << errstr);
//    }
//
//    /*设置consumer group*/
//    if (conf->set("group.id", groupid, errstr) != RdKafka::Conf::CONF_OK)
//    {
//        LOGERROR("RdKafka conf set group.id failed : " << errstr);
//    }
//
//    std::string strfetch_num = "10240000";
//    /*每次从单个分区中拉取消息的最大尺寸*/
//    if (conf->set("max.partition.fetch.bytes", strfetch_num, errstr) != RdKafka::Conf::CONF_OK)
//    {
//        LOGERROR("RdKafka conf set max.partition failed" << errstr);
//    }
//
//    /*创建kafka consumer实例*/
//    kafka_consumer_ = RdKafka::Consumer::create(conf, errstr);
//    if (!kafka_consumer_)
//    {
//        LOGERROR("failed to ceate consumer");
//    }
//    delete conf;
//
//    RdKafka::Conf *tconf = NULL;
//    /*创建kafka topic的配置*/
//    tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
//    if (!tconf)
//    {
//        LOGERROR("RdKafka create topic conf failed");
//        return false;
//    }
//
//    /*kafka + zookeeper,当消息被消费时,会想zk提交当前groupId的consumer消费的offset信息,
//    当consumer再次启动将会从此offset开始继续消费.在consumter端配置文件中(或者是
//    ConsumerConfig类参数)有个"autooffset.reset"(在kafka 0.8版本中为auto.offset.reset),
//    有2个合法的值"largest"/"smallest",默认为"largest",此配置参数表示当此groupId下的消费者,
//    在ZK中没有offset值时(比如新的groupId,或者是zk数据被清空),consumer应该从哪个offset开始
//    消费.largest表示接受接收最大的offset(即最新消息),smallest表示最小offset,即从topic的
//    开始位置消费所有消息.*/
//    if (tconf->set("auto.offset.reset", "smallest", errstr) != RdKafka::Conf::CONF_OK)
//    {
//        LOGERROR("RdKafka conf set auto.offset.reset failed : " << errstr);
//    }
//
//    topic = RdKafka::Topic::create(kafka_consumer_, topics, tconf, errstr);
//    if (!topic)
//    {
//        LOGERROR("RdKafka create topic failed : " << errstr);
//    }
//    delete tconf;
//
//  /**  RdKafka::ErrorCode resp = kafka_consumer_->start(topics, partition_, offset_);
//    if (resp != RdKafka::ERR_NO_ERROR)
//    {
//        LOGERROR("failed to start consumer : " << RdKafka::err2str(resp));
//    }*/
//
//    LOGINFO("消费者连kafka完成!");
//    return true;
//}
//
//void KafkaCustomer::consumer(RdKafka::Message *message, void *opt)
//{
//    switch (message->err())
//    {
//    case RdKafka::ERR__TIMED_OUT:
//        break;
//    case RdKafka::ERR_NO_ERROR:
//        printf("%.*s\n",
//               static_cast<int>(message->len()),
//               static_cast<const char *>(message->payload()));
//        last_offset_ = message->offset();
//        break;
//    case RdKafka::ERR__PARTITION_EOF:
//        std::cerr << "%% Reached the end of the queue, offset: " << last_offset_ << std::endl;
//        break;
//    case RdKafka::ERR__UNKNOWN_TOPIC:
//    case RdKafka::ERR__UNKNOWN_PARTITION:
//        std::cerr << "Consume failed: " << message->errstr() << std::endl;
//        run_ = false;
//        break;
//    default:
//        std::cerr << "Consume failed: " << message->errstr() << std::endl;
//        run_ = false;
//        break;
//    }
//}
//
//bool KafkaCustomer::consume(int timeout_ms)
//{
//    RdKafka::Message *msg = NULL;
//
//    while (run_)
//    {
//        msg = kafka_consumer_->consume(topic, partition_, timeout_ms);
//        this->consumer(msg, NULL);
//        kafka_consumer_->poll(0);
//        delete msg;
//    }
//
//    kafka_consumer_->stop(topic, partition_);
//    if (topic)
//    {
//        delete topic;
//        topic = NULL;
//    }
//    if (kafka_consumer_)
//    {
//        delete kafka_consumer_;
//        kafka_consumer_ = NULL;
//    }
//
//    /*销毁kafka实例*/
//    RdKafka::wait_destroyed(5000);
//    return true;
//}
