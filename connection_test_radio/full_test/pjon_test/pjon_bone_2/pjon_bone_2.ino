#include <PJON.h>
PJON<SoftwareBitBang> bus(2); // <Strategy name> bus(selected device id)

void setup() {
  bus.strategy.set_pin(10);
  bus.begin();
  bus.send_repeatedly(4, "B", 2, 1000000); // Send B to device 2 every second
}

void loop() {
  bus.update();
};
