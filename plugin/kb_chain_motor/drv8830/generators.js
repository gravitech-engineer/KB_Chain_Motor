Blockly.JavaScript['drv8830.speed'] = function(block) {
	var argument0 = Blockly.JavaScript.valueToCode(block, 'SPEED', Blockly.JavaScript.ORDER_ASSIGNMENT) || '0';
	return 'DEV_I2C1.DRV8830(0, ' + block.getFieldValue('ADDRESS') + ').speed((int)' + argument0 + ');\n';
};
