#ifndef __DRV8830_H__
#define __DRV8830_H__

#include "driver.h"
#include "device.h"
#include "i2c-dev.h"

typedef enum h_bridge_logic_struct {
	hbl_standby, hbl_reverse, hbl_forward, hbl_brake
} h_bridge_logic_t;

class DRV8830 : public Device {
	private:
		enum {
			s_detect, s_idle, s_error, s_wait
		} state;
		TickType_t tickcnt;
		uint8_t flag;
		uint8_t control_byte_shadow, dac_in_percent, dac_in_percent_shadow;
		h_bridge_logic_t h_bridge_logic, h_bridge_logic_shadow;
		uint8_t get_control_byte(uint8_t percent_dac, h_bridge_logic_t logic_h_bridge);

	public:
		// constructor
		DRV8830(int bus_ch, int dev_addr);
		// override
		void init(void);
		void process(Driver *drv);
		int prop_count(void);
		bool prop_name(int index, char *name);
		bool prop_unit(int index, char *unit);
		bool prop_attr(int index, char *attr);
		bool prop_read(int index, char *value);
		bool prop_write(int index, char *value);
		// method
		//void set_dac_and_h_bridge(uint8_t percent_dac, h_bridge_logic_t logic_h_bridge);
		//void set_dac(uint8_t percent_dac);
		//void set_h_bridge(h_bridge_logic_t logic_h_bridge);
		void speed(int percent);
};

#endif
