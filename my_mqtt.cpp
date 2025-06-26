#include "my_mqtt.h"
#include <cstdio>


volatile int arrivedcount = 0;

TCPSocket socket;
MQTTClient client(&socket);

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\r\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\r\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
}

void mqtt_send(void* databuf, int len, const char* topic){
    // printf("skdjsaldjalkjdlaskj");
    MQTT::Message msg;
    msg.payload = databuf;
    msg.payloadlen = len;

    msg.dup = false;
    msg.qos = MQTT::QOS0;
    msg.retained = false;
    client.publish(topic, msg);

}
void mqtt_demo(NetworkInterface *net)
{
    float version = 0.6;
 

 
    SocketAddress a;
    const char* hostname = "dev.rightech.io";
    net->gethostbyname(hostname, &a);
    int port = 1883;
    a.set_port(port);
 
    printf("Connecting to %s:%d\r\n", hostname, port);
 
    socket.open(net);
    printf("Opened socket\n\r");
    int rc = socket.connect(a);
    if (rc != 0)
        printf("rc from TCP connect is %d\r\n", rc);
    printf("Connected socket\n\r");
 
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "mqtt-udevrules-8ctx6t";
    data.username.cstring = "123";
    data.password.cstring = "123";
    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\r\n", rc);
 
    if ((rc = client.subscribe("distance", MQTT::QOS2, messageArrived)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);
    if ((rc = client.subscribe("rfid", MQTT::QOS2, messageArrived)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);
    
    return;
}