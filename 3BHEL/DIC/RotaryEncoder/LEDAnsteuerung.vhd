----------------------------------------------------------------------------------
-- Company:         HTL Hollabrunn
-- Engineer:        < Platajs Martin >
--
-- Create Date:     2019-05-05
-- Module Name:     LED-Ansteuerung
-- Project Name:    LED-Ansteuerung
-- Description: 	  Ansteuerung für die LEDs des Rotary-K.I.T.T.-Lauflichts 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
-- use IEEE.NUMERIC_STD.ALL;

entity LEDAnsteuerung is
port(Clk, RstN : in std_logic;								-- clk-> pos. Taktflanke; RstN -> aktiv 0 asynchron
	  TR, TL : in std_logic;
	  LED : out std_logic_vector(7 downto 0):= "10000000");
end LEDAnsteuerung;

architecture Reg of LEDAnsteuerung is
signal LED_buf: std_logic_vector (7 downto 0);				-- Buffer Vektor für LEDs

begin
	LED <= LED_buf;											-- NLA zum übernehmen der Position der LEDs
	RegP : process(Clk, RstN)
	begin
      if RstN = '0' then
         LED_buf <= (0 => '1', others => '0');				-- asynchroner aktiv 0 Reset (nur linke LED leuchtet)
      elsif CLK'event and CLK = '1' then
         if TL = '1' and LED_buf(7) = '0' then				-- um Tick-Left zu erkennen 
            LED_buf <= LED_buf(6 downto 0) & LED_buf(7);
         elsif TR = '1' and LED_buf(0) = '0' then			-- um Tick-Right zu erkennen
            LED_buf <= LED_buf(0) & LED_buf(7 downto 1);
         end if;
      end if;
	end process RegP;
end Reg;