library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

entity address_decoder is
	Port (
		a11 : In std_logic;
		a10 : In std_logic;
		
		ram_cs  : Out std_logic;
		adc_cs  : Out std_logic;
		oled_cs : Out std_logic
	);
	attribute LOC : string;
	attribute LOC of a11 : signal is "P1";
	attribute LOC of a10 : signal is "P2";
	
	attribute LOC of ram_cs	: signal is "P19";
	attribute LOC of adc_cs : signal is "P18";
	attribute LOC of oled_cs : signal is "P17";
end address_decoder;

architecture behave of address_decoder is
begin
-- implement the functionality here
	
	-- Partial decoding used. Only MSB of address (a10 & a11) are used
	-- All Chip Selects (CS) of ICs are logically negated (inverted)
	oled_cs <= '0' when (a11 = '0' and a10 = '0')
				else '1';
			   
	adc_cs 	<= '0' when (a11 = '0' and a10 = '1')
				else '1';
			   
	ram_cs 	<= '0' when (a11 = '1')
				else '1';

end behave;
