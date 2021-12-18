----------------------------------------------------------------------------------
-- Company:         HTL Hollabrunn
-- Engineer:        Platajs Martin lfd. Nr. 15 3BHEL
--
-- Create Date:     2019-1-13
-- Module Name:     shift_register
-- Project Name:    LAUFLICHT  
-- Description: 	  8-Bit Schieberegister mit LEDs veranschaulicht
--
-- 
--
-- Revision:        V0.1
--                   | +--- Nummer der Ausgabe
--                   +----- 0 -> In Entwicklung, 1 -> Produktiv    
--
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity shift_register is
	Port ( clk    : in std_logic;					-- Clk 20 MHz von Max1000 gegeben
		   rstN	  : in std_logic := '1';		-- asynchroner aktiv low Reset
		   button : in std_logic;					-- Taster zum weiterschieben der Daten im register
		   LED : out std_logic_vector(7 downto 0));		-- LED Ausgangs Vector für die Zuweisung der LEDs
end shift_register;												-- am Max1000 board

architecture behavioral of shift_register is
		signal shift_reg : std_logic_vector(7 downto 0);		-- Signal Vector für eigentliches Schieberegister
		signal button_prev : std_logic := '0';			-- signal zur Feststellung des vorigen Tastenzustands
		
begin	
		shift : process (clk,rstN)		-- Prozess zum weiterschieben der Daten im Schieberegister
			begin
				if(rstN = '0')then		-- asynchroner aktiv low Reset
					shift_reg <= "00000001";	-- Bei rstN = '0' soll 1. LED leuchten
					button_prev <= '0';			-- Der gespeicherte Zustand des Tasters wird auf 0 gesetzt
				elsif (clk'event and clk = '1')then		--Bei steigender Flanke werden Tastendrücke übernommen
					if(button = '1' and button_prev = '0')then	-- Damit bei einem Tastendruck nicht mehrmals 
																				-- weitergeschoben wird
						shift_reg <= shift_reg(6 downto 0) & shift_reg(7);		--Das Weiterschieben
					end if;
					button_prev <= button;		--Speichern vom Zustand des Tasters
				end if;
			end process shift;
			LED <= shift_reg;		--NLA um Schieberegister auf LEDs zu übernehmen		
end behavioral;