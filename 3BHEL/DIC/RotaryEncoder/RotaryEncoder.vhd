----------------------------------------------------------------------------------
-- Company:         HTL Hollabrunn
-- Engineer:        < Platajs Martin >
--
-- Create Date:     2019-05-05
-- Module Name:     Rotary-Encoder
-- Project Name:    Rotary-Encoder
-- Description: 	Codefolge-Erkennung des Inkrementalgebers 
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

entity RotaryEncoder is
port(Clk, RstN : in std_logic;							-- clk-> pos. Taktflanke; RstN -> aktiv 0 asynchron
		A, B : in std_logic;
		TR, TL : out std_logic);
end RotaryEncoder;

architecture RotaryEncoderbehaviour of RotaryEncoder is
type StateType is (Idle ,L1, L2, L, R1, R2, R);
signal CurState : Statetype;
signal NextState : Statetype;
begin
		RegP : process (Clk, RstN)						--Register process (synchron)
		begin
		
			if(RstN = '0') then
			
				CurState <= Idle;						--Set reset state -see state diagramm
			
			elsif(Clk'event and Clk = '1')then
				
				curState <= NextState;	
				
			end if;
		end process RegP;
		
	NSLP : process ( A, B, CurState)					--Next state logic
		begin
			NEXTSTATE <= CURSTATE;                    -- Default assignment of next state
  
      case CURSTATE is
  	      when IDLE =>
            if (A = '0') and (B = '1') then
               NEXTSTATE <= L1;
            elsif A = '1' and B = '0' then
               NEXTSTATE <= R1;
            end if;
				
  		   when L1 =>
            if A = '1' and B = '1' then
               NEXTSTATE <= L2;
            elsif A = '0' and B = '0' then
               NEXTSTATE <= IDLE;
            end if;
  		   when L2 =>
            if A = '0' and B = '0' then
               NEXTSTATE <= L;
            elsif A = '0' and B = '1' then
               NEXTSTATE <= L1;
            end if;
  		   when L =>
  		   	NEXTSTATE <= IDLE;
				
			when R1 =>
            if A = '1' and B = '1' then
               NEXTSTATE <= R2;
            elsif A = '0' and B = '0' then
               NEXTSTATE <= IDLE;
            end if;
  		   when R2 =>
            if A = '0' and B = '0' then
               NEXTSTATE <= R;
            elsif A = '1' and B = '0' then
               NEXTSTATE <= R1;
            end if;
  		   when R =>
  		   	NEXTSTATE <= IDLE;
			
         when others =>
            NEXTSTATE <= IDLE;
      end case;
			
		end process NSLP;
   OUTp : process (CURSTATE)
   begin
      case CURSTATE is
         when L => TL <= '1'; TR <= '0';
         when R => TL <= '0'; TR <= '1';
         when others => TL <= '0'; TR <= '0';
      end case;
   end process OUTp;
	end RotaryEncoderbehaviour;