#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <err.h>

#include "mosquitto.h"

#define KEEPALIVE_INTERVAL 60

static void on_connect(struct mosquitto* mosq, void* obj, int rc);
static void on_publish(struct mosquitto* mosq, void* obj, int mid);
static void on_subscribe(struct mosquitto* mosq, void* obj, int mid, int qos_count, const int* granted_qos);
static void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message);

int main(int argc, char* argv[]) {
	struct mosquitto* mosq = NULL;
	int res = 0;

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, true, NULL);
	if (mosq == NULL) {
		goto CLEANUP;
	}

	mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_publish_callback_set(mosq, on_publish);
    mosquitto_subscribe_callback_set(mosq, on_subscribe);
    mosquitto_message_callback_set(mosq, on_message);

    res = mosquitto_username_pw_set(mosq, _MQTT_USER, _MQTT_PASSWORD);
    if (res != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "ERROR: mosquitto_username_pw_set: %s\n", mosquitto_strerror(res));    		
    	goto CLEANUP;
    }

    printf("Connecting to %s:%d\n", _BROKER_HOSTNAME, _BROKER_PORT);
    res = mosquitto_connect(mosq, _BROKER_HOSTNAME, _BROKER_PORT, KEEPALIVE_INTERVAL);
    if (res != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "ERROR: mosquitto_connect: %s\n", mosquitto_strerror(res));    		
    	goto CLEANUP;
    }

    // res = mosquitto_loop_forever(mosq, 1000, 1000);
    res = mosquitto_loop_start(mosq);
    if (res != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "ERROR: mosquitto_loop_start: %s\n", mosquitto_strerror(res));    		
    	goto CLEANUP;
    }
    printf("Loop started\n");

    char buf[BUFSIZ];
    for (;;) {
    	char* msg = fgets(buf, BUFSIZ-1, stdin);
        buf[BUFSIZ] = '\0';
        msg = buf;
        char last_ch = msg[strlen(msg)-1];
        while (last_ch == '\n' || last_ch == '\r') {
            msg[strlen(msg)-1] = '\0'; 
            last_ch = msg[strlen(msg)-1];
        }
        // printf("Publising %s\n", msg);
    	res = mosquitto_publish(mosq, NULL, _MQTT_TOPIC, strlen(msg), msg, 0, true);
    	if (res != MOSQ_ERR_SUCCESS) {
			fprintf(stderr, "ERROR: mosquitto_publish: %s\n", mosquitto_strerror(res));    		
    	}
    }

CLEANUP:
	if (mosq != NULL) {
	    mosquitto_destroy(mosq);
	}
    mosquitto_lib_cleanup();

    return 0;
}

void on_connect(struct mosquitto* mosq, void* obj, int rc) {
	// printf("Connected\n");
	if (rc == 0) {
		printf("Subscribe to %s\n", _MQTT_TOPIC);
		mosquitto_subscribe(mosq, NULL, _MQTT_TOPIC, 0);
	}
}

void on_publish(struct mosquitto* mosq, void* obj, int mid) {
    // printf("Published\n");
}

void on_subscribe(struct mosquitto* mosq, void* obj, int mid, int qos_count, const int* granted_qos) {
    // printf("Subscribed\n");
}

void on_message(struct mosquitto* mosq, void* obj, const struct mosquitto_message* message) {
    // printf("Received a message\n");
	if (message) {
		printf("<<< %s\n", (char*)message->payload);
	}
}
