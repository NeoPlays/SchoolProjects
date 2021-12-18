----------------------------------------------------------------------------------
-- Company:         HTL Hollabrunn
-- Engineer:        < Platajs Martin >
--
-- Create Date:     2019-05-05
-- Module Name:     Tick_Gen
-- Project Name:    Tick_Gen
-- Description: 	Tick Generator
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

entity Tick_gen is
generic ( Div : natural := 30 );			-- Konstante nach wievielen clk-Perioden ein Tick ausgegeben wird.
	port( clk, RstN : in std_logic;		-- clk-> pos. Taktflanke; RstN -> aktiv 0 asynchron
			Tick : out std_logic);
end Tick_Gen;

architecture UseNatural of Tick_Gen is

signal Count : natural range 0 to Div-1;

begin

COUNTP : process ( clk, RstN )
	begin
		if(RstN = '0') then					-- asynchroner aktiv 0 Reset
			Count <= 0;
		
		elsif ( clk'event and clk = '1' ) then			
			if (Count = 0) then
				 Count <= Div - 1;			-- um nach dem Tick von neu zu beginnen
			else
				 Count <= Count - 1;			
			
			end if;
		end if;
	
	end process COUNTP;

Tick <= '1' when ( Count = 0 ) else '0';		--NLA zum übernehmen des Ticks

end UseNatural;