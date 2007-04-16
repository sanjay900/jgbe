import java.io.*;
public class romtester {
	public static void main( String[] args ) {
		CPU cpu;
		Cartridge cartridge;
		String romfile="", logfile="";
		for (int i = 0; i < args.length; ++i) {
			if (args[i].charAt(0)!='-')
				romfile = args[i];
			if (args[i].equals("-log"))
				logfile = args[++i];
		}
		if (romfile.equals("")) {
			System.out.println();
			System.out.println("ERROR: missing argument");
			System.out.println();
			System.out.println("USAGE: java swinggui ROMFILE [-log LOGFILE]");
			System.out.println();
			return;
		}
		Writer logwriter = null;
		try {
			if (!logfile.equals(""))
				logwriter = new BufferedWriter( new FileWriter(logfile) );
		}
		catch (java.io.IOException e) {
			System.out.println("Error opening logfile:" + e.getMessage());
			logwriter = null;
		}

		cartridge = new Cartridge(romfile);
		if(cartridge.getError()!=null) {
			System.out.println("ERROR: "+cartridge.getError());
			return;
		}

		System.out.println("Succesfully loaded ROM :)");
		cpu = new CPU(cartridge);

		cpu.reset();
		cpu.AC.isMuted = true;

		int instrlimit=100;

		while (true) {
			if (logwriter != null) {
				String out = String.format("PC=$%04x AF=$%02x%02x BC=$%02x%02x DE=$%02x%02x HL=$%02x%02x SP=$%04x\n",
					cpu.PC,
					cpu.A,
					cpu.F,
					cpu.B,
					cpu.C,
					cpu.D,
					cpu.E,
					cpu.H,
					cpu.L,
					cpu.SP);
				try {
					logwriter.write(out);
				}
				catch (java.io.IOException e) {
					System.out.println("Error writing logfile:" + e.getMessage());
					logwriter = null;
				}
			}
			if ((--instrlimit==0) || cpu.nextinstruction()==0) {
				String s= (new String()).format("%02x",cpu.read(cpu.PC));
				String ss= (new String()).format("%04x",cpu.PC);
				s=s.toUpperCase();
				ss=ss.toUpperCase();
				if (logwriter != null) {
					String out = String.format("invalid opcode 0x"+s+" at address 0x"+ss+", rombank = "+cartridge.CurrentROMBank+"\n");
					try {
						logwriter.write(out);
					}
					catch (java.io.IOException e) {
						System.out.println("Error writing logfile:" + e.getMessage());
						logwriter = null;
					}
				}
				try {
					logwriter.flush();
				}
				catch (java.io.IOException e2) {
					System.out.println("Error flushing logfile:" + e2.getMessage());
					logwriter = null;
				}
				return;
			}
		}
	}
}
