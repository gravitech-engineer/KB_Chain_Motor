#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "kidbright32.h"
#include "drv8830.h"

/*
Commanded Output Voltage = 4 x VREF x (VSET +1) / 64, where VREF is the internal 1.285-V

IN1 IN2  OUT1 OUT2		FUNCTION
0    0    Z    Z		Standby / coast
0    1    L    H		Reverse
1    0    H    L		Forward
1    1    H    H		Brake
*/

// drv8830 registers
#define DRV8830_REG_CONTROL				0x00
#define DRV8830_REG_FAULT				0x01

#define DRV8830_WRITE_FLAG				0x01

DRV8830::DRV8830(int bus_ch, int dev_addr) {
	channel = bus_ch;
	address = dev_addr;
}

void DRV8830::init(void) {
	flag = 0;
	dac_in_percent = 0;
	h_bridge_logic = hbl_standby;
	state = s_detect;
}

int DRV8830::prop_count(void) {
	// not supported
	return 0;
}

bool DRV8830::prop_name(int index, char *name) {
	// not supported
	return false;
}

bool DRV8830::prop_unit(int index, char *unit) {
	// not supported
	return false;
}

bool DRV8830::prop_attr(int index, char *attr) {
	// not supported
	return false;
}

bool DRV8830::prop_read(int index, char *value) {
	// not supported
	return false;
}

bool DRV8830::prop_write(int index, char *value) {
	// not supported
	return false;
}

uint8_t DRV8830::get_control_byte(uint8_t percent_dac, h_bridge_logic_t logic_h_bridge) {
	uint8_t val;

	if (percent_dac > 100) {
		percent_dac = 100;
	}
	if (percent_dac == 0) {
		logic_h_bridge = hbl_brake;
	}

	val = (percent_dac * 0x3f) / 100;
	// cap dac at 0x06 (0.48 V) to 0x3e (4.98 V)
	if (val < 0x06) {
		val = 0x06;
	}
	if (val > 0x3e) {
		val = 0x3e;
	}

	// save in shadow variables
	dac_in_percent_shadow = percent_dac;
	h_bridge_logic_shadow = logic_h_bridge;

	return (val << 2) | (logic_h_bridge & 0x03);
}

void DRV8830::process(Driver *drv) {
	I2CDev *i2c = (I2CDev *)drv;
	uint8_t data[4];
	uint8_t dac;

	switch (state) {
		case s_detect:
			// detect i2c device
			if (i2c->detect(channel, address) == ESP_OK) {
				// init drv8830
				data[0] = DRV8830_REG_CONTROL;
				data[1] = get_control_byte(0, hbl_brake);
				if (i2c->write(channel, address, data, 2) == ESP_OK) {
					// update current dac and h bridge state
					dac_in_percent = dac_in_percent_shadow;
					h_bridge_logic = h_bridge_logic_shadow;
					// set initialized flag
					initialized = true;
					// clear error flag
					error = false;
					//state = s_cmd_init;
					state = s_idle;
				}
				else {
					state = s_error;
				}
			}
			else {
				state = s_error;
			}
			break;

		case s_idle:
			// check pending flag
			if (IS_FLAG_SET(flag, DRV8830_WRITE_FLAG)) {
				FLAG_CLR(flag, DRV8830_WRITE_FLAG);
				// write to drv8830
				data[0] = DRV8830_REG_CONTROL;
				data[1] = control_byte_shadow;
				if (i2c->write(channel, address, data, 2) == ESP_OK) {
					// update current dac and h bridge state
					dac_in_percent = dac_in_percent_shadow;
					h_bridge_logic = h_bridge_logic_shadow;
				}
				else {
					state = s_error;
				}
			}
			break;

		case s_error:
			// set error flag
			error = true;
			// clear initialized flag
			initialized = false;
			// get current tickcnt
			tickcnt = get_tickcnt();
			// goto wait and retry with detect state
			state = s_wait;
			break;

		case s_wait:
			// delay 1000ms before retry detect
			if (is_tickcnt_elapsed(tickcnt, 1000)) {
				state = s_detect;
			}
			break;
	}
}
/*
void DRV8830::set_dac_and_h_bridge(uint8_t percent_dac, h_bridge_logic_t logic_h_bridge) {
	// calc control byte
	control_byte_shadow = get_control_byte(percent_dac, logic_h_bridge);
	// set write flag
	FLAG_SET(flag, DRV8830_WRITE_FLAG);
}

void DRV8830::set_dac(uint8_t percent_dac) {
	set_dac_and_h_bridge(percent_dac, h_bridge_logic);
}

void DRV8830::set_h_bridge(h_bridge_logic_t logic_h_bridge) {
	set_dac_and_h_bridge(dac_in_percent, logic_h_bridge);
}
*/
void DRV8830::speed(int percent) {
	if ((percent != 0) && ((percent >= -100) && (percent <= 100))) {
		if (percent < 0) {
			// calc control byte
			control_byte_shadow = get_control_byte((-1 * percent), hbl_reverse);
		}
		else {
			// calc control byte
			control_byte_shadow = get_control_byte(percent, hbl_forward);
		}
	}
	else {
		// calc control byte
		control_byte_shadow = get_control_byte(0, hbl_standby);
	}
	// set write flag
	FLAG_SET(flag, DRV8830_WRITE_FLAG);
}
