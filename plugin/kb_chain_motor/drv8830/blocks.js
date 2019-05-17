Blockly.Blocks["drv8830.speed"] = {
	init: function() {
		this.appendDummyInput()
			.appendField(Blockly.Msg.DRV8830_SPEED_TITLE);

		// i2c channel 0 to 64
/*		var channel_array = [];
		for (var i = 0;i <= 64; i++) {
			channel_array.push([String(i), String(i)]);
		}
		this.appendDummyInput()
			.appendField(Blockly.Msg.CHANNEL)
			.appendField(new Blockly.FieldDropdown(channel_array), 'CHANNEL');*/

		// device addess
		this.appendDummyInput()
			.appendField(Blockly.Msg.MOTOR)
			.appendField(new Blockly.FieldDropdown( [
/*				["0x60", "96"],
				["0x61", "97"],
				["0x62", "98"],
				["0x63", "99"],
				["0x64", "100"],
				["0x65", "101"],
				["0x66", "102"],
				["0x67", "103"],
				["0x68", "104"]*/
				["1", "102"],
				["2", "104"]
			]), 'ADDRESS');

		this.appendValueInput("SPEED")
			.appendField(Blockly.Msg.MOTOR_SPEED + '(%)')
			.setCheck('Number');

/*		this.appendDummyInput()
			.appendField(Blockly.Msg.MOTOR_STATE).appendField(new Blockly.FieldDropdown([
				[Blockly.Msg.MOTOR_STANDBY, "hbl_standby"],
				[Blockly.Msg.MOTOR_REVERSE, "hbl_reverse"],
				[Blockly.Msg.MOTOR_FORWARD, "hbl_forward"],
				[Blockly.Msg.MOTOR_BRAKE, "hbl_brake"]
			]), 'STATE');*/

		this.setInputsInline(true);
		this.setPreviousStatement(true);
		this.setNextStatement(true);
		this.setColour(160);
		this.setTooltip(Blockly.Msg.DRV8830_SPEED_TOOLTIP);
		this.setHelpUrl(Blockly.Msg.DRV8830_SPEED_HELPURL);
	}
};
