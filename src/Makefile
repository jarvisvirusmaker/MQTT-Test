MOSQ_DIST = ../../mosquitto_dist

COMPILER = g++
CFLAGS   = -g -MMD -MP -Wall -Wextra -Winit-self -Wno-unused-parameter -Wfloat-equal \
  -D_BROKER_HOSTNAME=\"lite.mqtt.shiguredo.jp\" \
  -D_BROKER_PORT=1883 \
  -D_MQTT_TOPIC=\"__YOUR_TOPIC__\" \
  -D_MQTT_USER=\"__YOUR_USER_NAME__\" \
  -D_MQTT_PASSWORD=\"__YOUR_PASSWORD__\"
LDFLAGS  =
LIBS     = $(MOSQ_DIST)/usr/local/lib/libmosquitto.so.1
INCLUDE  = -I$(MOSQ_DIST)/usr/local/include
TARGET   = ../bin/$(shell basename `readlink -f ..`)
OBJDIR   = ./obj
ifeq "$(strip $(OBJDIR))" ""
  OBJDIR = .
endif
SOURCES  = $(wildcard *.c)
OBJECTS  = $(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))
DEPENDS  = $(OBJECTS:.o=.d)


$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	@[ -d $(OBJDIR) ] || mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(TARGET)
	@rmdir --ignore-fail-on-non-empty `readlink -f $(OBJDIR)`

-include $(DEPENDS)
