library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity xillydemo is
  port (
    PCIE_PERST_B_LS : IN std_logic;
    PCIE_REFCLK_N : IN std_logic;
    PCIE_REFCLK_P : IN std_logic;
    PCIE_RX_N : IN std_logic_vector(7 DOWNTO 0);
    PCIE_RX_P : IN std_logic_vector(7 DOWNTO 0);
    GPIO_LED : OUT std_logic_vector(3 DOWNTO 0);
    PCIE_TX_N : OUT std_logic_vector(7 DOWNTO 0);
    PCIE_TX_P : OUT std_logic_vector(7 DOWNTO 0));
end xillydemo;

architecture sample_arch of xillydemo is
  component xillybus
    port (
      PCIE_PERST_B_LS : IN std_logic;
      PCIE_REFCLK_N : IN std_logic;
      PCIE_REFCLK_P : IN std_logic;
      PCIE_RX_N : IN std_logic_vector(7 DOWNTO 0);
      PCIE_RX_P : IN std_logic_vector(7 DOWNTO 0);
      GPIO_LED : OUT std_logic_vector(3 DOWNTO 0);
      PCIE_TX_N : OUT std_logic_vector(7 DOWNTO 0);
      PCIE_TX_P : OUT std_logic_vector(7 DOWNTO 0);
      bus_clk : OUT std_logic;
      quiesce : OUT std_logic;
      user_r_mem_8_rden : OUT std_logic;
      user_r_mem_8_empty : IN std_logic;
      user_r_mem_8_data : IN std_logic_vector(7 DOWNTO 0);
      user_r_mem_8_eof : IN std_logic;
      user_r_mem_8_open : OUT std_logic;
      user_w_mem_8_wren : OUT std_logic;
      user_w_mem_8_full : IN std_logic;
      user_w_mem_8_data : OUT std_logic_vector(7 DOWNTO 0);
      user_w_mem_8_open : OUT std_logic;
      user_mem_8_addr : OUT std_logic_vector(4 DOWNTO 0);
      user_mem_8_addr_update : OUT std_logic;
      user_r_read_32_rden : OUT std_logic;
      user_r_read_32_empty : IN std_logic;
      user_r_read_32_data : IN std_logic_vector(31 DOWNTO 0);
      user_r_read_32_eof : IN std_logic;
      user_r_read_32_open : OUT std_logic;
      user_r_read_8_rden : OUT std_logic;
      user_r_read_8_empty : IN std_logic;
      user_r_read_8_data : IN std_logic_vector(7 DOWNTO 0);
      user_r_read_8_eof : IN std_logic;
      user_r_read_8_open : OUT std_logic;
      user_w_write_32_wren : OUT std_logic;
      user_w_write_32_full : IN std_logic;
      user_w_write_32_data : OUT std_logic_vector(31 DOWNTO 0);
      user_w_write_32_open : OUT std_logic;
      user_w_write_8_wren : OUT std_logic;
      user_w_write_8_full : IN std_logic;
      user_w_write_8_data : OUT std_logic_vector(7 DOWNTO 0);
      user_w_write_8_open : OUT std_logic);
  end component;

  component fifo_8x2048
    port (
      clk: IN std_logic;
      srst: IN std_logic;
      din: IN std_logic_VECTOR(7 downto 0);
      wr_en: IN std_logic;
      rd_en: IN std_logic;
      dout: OUT std_logic_VECTOR(7 downto 0);
      full: OUT std_logic;
      empty: OUT std_logic);
  end component;

  component fifo_32x512
    port (
      clk: IN std_logic;
      srst: IN std_logic;
      din: IN std_logic_VECTOR(31 downto 0);
      wr_en: IN std_logic;
      rd_en: IN std_logic;
      dout: OUT std_logic_VECTOR(31 downto 0);
      full: OUT std_logic;
      empty: OUT std_logic);
  end component;

-- Synplicity black box declaration
  attribute syn_black_box : boolean;
  attribute syn_black_box of fifo_32x512: component is true;
  attribute syn_black_box of fifo_8x2048: component is true;

  type demo_mem is array(0 TO 31) of std_logic_vector(7 DOWNTO 0);
  signal demoarray : demo_mem;
  
  signal bus_clk :  std_logic;
  signal quiesce : std_logic;

  signal reset_8 : std_logic;
  signal reset_32 : std_logic;

  signal ram_addr : integer range 0 to 31;
  
  signal user_r_mem_8_rden :  std_logic;
  signal user_r_mem_8_empty :  std_logic;
  signal user_r_mem_8_data :  std_logic_vector(7 DOWNTO 0);
  signal user_r_mem_8_eof :  std_logic;
  signal user_r_mem_8_open :  std_logic;
  signal user_w_mem_8_wren :  std_logic;
  signal user_w_mem_8_full :  std_logic;
  signal user_w_mem_8_data :  std_logic_vector(7 DOWNTO 0);
  signal user_w_mem_8_open :  std_logic;
  signal user_mem_8_addr :  std_logic_vector(4 DOWNTO 0);
  signal user_mem_8_addr_update :  std_logic;
  signal user_r_read_32_rden :  std_logic;
  signal user_r_read_32_empty :  std_logic;
  signal user_r_read_32_data :  std_logic_vector(31 DOWNTO 0);
  signal user_r_read_32_eof :  std_logic;
  signal user_r_read_32_open :  std_logic;
  signal user_r_read_8_rden :  std_logic;
  signal user_r_read_8_empty :  std_logic;
  signal user_r_read_8_data :  std_logic_vector(7 DOWNTO 0);
  signal user_r_read_8_eof :  std_logic;
  signal user_r_read_8_open :  std_logic;
  signal user_w_write_32_wren :  std_logic;
  signal user_w_write_32_full :  std_logic;
  signal user_w_write_32_data :  std_logic_vector(31 DOWNTO 0);
  signal user_w_write_32_open :  std_logic;
  signal user_w_write_8_wren :  std_logic;
  signal user_w_write_8_full :  std_logic;
  signal user_w_write_8_data :  std_logic_vector(7 DOWNTO 0);
  signal user_w_write_8_open :  std_logic;

begin
  xillybus_ins : xillybus
    port map (
      -- Ports related to /dev/xillybus_mem_8
      -- FPGA to CPU signals:
      user_r_mem_8_rden => user_r_mem_8_rden,
      user_r_mem_8_empty => user_r_mem_8_empty,
      user_r_mem_8_data => user_r_mem_8_data,
      user_r_mem_8_eof => user_r_mem_8_eof,
      user_r_mem_8_open => user_r_mem_8_open,
      -- CPU to FPGA signals:
      user_w_mem_8_wren => user_w_mem_8_wren,
      user_w_mem_8_full => user_w_mem_8_full,
      user_w_mem_8_data => user_w_mem_8_data,
      user_w_mem_8_open => user_w_mem_8_open,
      -- Address signals:
      user_mem_8_addr => user_mem_8_addr,
      user_mem_8_addr_update => user_mem_8_addr_update,

      -- Ports related to /dev/xillybus_read_32
      -- FPGA to CPU signals:
      user_r_read_32_rden => user_r_read_32_rden,
      user_r_read_32_empty => user_r_read_32_empty,
      user_r_read_32_data => user_r_read_32_data,
      user_r_read_32_eof => user_r_read_32_eof,
      user_r_read_32_open => user_r_read_32_open,

      -- Ports related to /dev/xillybus_read_8
      -- FPGA to CPU signals:
      user_r_read_8_rden => user_r_read_8_rden,
      user_r_read_8_empty => user_r_read_8_empty,
      user_r_read_8_data => user_r_read_8_data,
      user_r_read_8_eof => user_r_read_8_eof,
      user_r_read_8_open => user_r_read_8_open,

      -- Ports related to /dev/xillybus_write_32
      -- CPU to FPGA signals:
      user_w_write_32_wren => user_w_write_32_wren,
      user_w_write_32_full => user_w_write_32_full,
      user_w_write_32_data => user_w_write_32_data,
      user_w_write_32_open => user_w_write_32_open,

      -- Ports related to /dev/xillybus_write_8
      -- CPU to FPGA signals:
      user_w_write_8_wren => user_w_write_8_wren,
      user_w_write_8_full => user_w_write_8_full,
      user_w_write_8_data => user_w_write_8_data,
      user_w_write_8_open => user_w_write_8_open,

      -- General signals
      PCIE_PERST_B_LS => PCIE_PERST_B_LS,
      PCIE_REFCLK_N => PCIE_REFCLK_N,
      PCIE_REFCLK_P => PCIE_REFCLK_P,
      PCIE_RX_N => PCIE_RX_N,
      PCIE_RX_P => PCIE_RX_P,
      GPIO_LED => GPIO_LED,
      PCIE_TX_N => PCIE_TX_N,
      PCIE_TX_P => PCIE_TX_P,
      bus_clk => bus_clk,
      quiesce => quiesce
      );

--  A simple inferred RAM

  ram_addr <= conv_integer(user_mem_8_addr);
  
  process (bus_clk)
  begin
    if (bus_clk'event and bus_clk = '1') then
      if (user_w_mem_8_wren = '1') then 
        demoarray(ram_addr) <= user_w_mem_8_data;
      end if;
      if (user_r_mem_8_rden = '1') then
        user_r_mem_8_data <= demoarray(ram_addr);
      end if;
    end if;
  end process;

  user_r_mem_8_empty <= '0';
  user_r_mem_8_eof <= '0';
  user_w_mem_8_full <= '0';

--  32-bit loopback

  fifo_32 : fifo_32x512
    port map(
      clk        => bus_clk,
      srst       => reset_32,
      din        => user_w_write_32_data,
      wr_en      => user_w_write_32_wren,
      rd_en      => user_r_read_32_rden,
      dout       => user_r_read_32_data,
      full       => user_w_write_32_full,
      empty      => user_r_read_32_empty
      );

  reset_32 <= not (user_w_write_32_open or user_r_read_32_open);

  user_r_read_32_eof <= '0';
  
--  8-bit loopback

  fifo_8 : fifo_8x2048
    port map(
      clk        => bus_clk,
      srst       => reset_8,
      din        => user_w_write_8_data,
      wr_en      => user_w_write_8_wren,
      rd_en      => user_r_read_8_rden,
      dout       => user_r_read_8_data,
      full       => user_w_write_8_full,
      empty      => user_r_read_8_empty
      );

    reset_8 <= not (user_w_write_8_open or user_r_read_8_open);

    user_r_read_8_eof <= '0';
  
end sample_arch;
