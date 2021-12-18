LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY shift_register_vhd_tst IS
END shift_register_vhd_tst;
ARCHITECTURE shift_register_arch OF shift_register_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL button : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL LED : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL rstN : STD_LOGIC;
COMPONENT shift_register
	PORT (
	button : IN STD_LOGIC;
	clk : IN STD_LOGIC;
	LED : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	rstN : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : shift_register
	PORT MAP (
-- list connections between master ports and signals
	button => button,
	clk => clk,
	LED => LED,
	rstN => rstN
	);
	
clkP: process			--Prozess der den Clock simuliert(keine 20MHz)
begin 
	clk <= '0';
	wait for 50 ns;
	clk <= '1';
	wait for 50 ns;
end process clkP;

resetP : process		--Prozess der den Reset-Tastendruck simuliert
begin
	rstN <= '0';
	wait for 50 ns;
	rstN <= '1';
	wait for 500 ns;
	rstN <= '0';
	wait for 50 ns;
	rstN <= '1';
	wait;
end process resetP;

buttonP : process		--Prozess der den Tastendruck simuliert
begin
		button <= '0';
		wait for 175 ns;
		button <= '1';
		wait for 300 ns;
end process buttonP;
                                                                                                
END shift_register_arch;
