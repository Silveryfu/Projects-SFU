import java.io.*;
import java.nio.*;
import java.nio.channels.FileChannel;

public class Base64Encoder {
	private static final int BSIZE = 3;
	private static final int LINECUT = 76;
	private static final char[] indexTable = { 'A', 'B', 'C', 'D', 'E', 'F',
			'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
			'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
			't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9', '+', '/' };

	@SuppressWarnings("resource")
	public static void encode() throws IOException {
		FileChannel fc = new FileInputStream("data.in").getChannel();
		ByteBuffer bb = ByteBuffer.allocate(BSIZE);
		StringBuilder sb = new StringBuilder();
		short s_fake = 0;
		int count = 0;
		while (fc.read(bb) != -1) {
			if (bb.position() == 3) {
				if (count % LINECUT == 0 && count != 0)
					sb.append("\n");
				bb.flip();
				s_fake = bb.getShort(); // used to get 2 bytes
				sb.append(indexTable[s_fake >> 10 & 0x3f]);
				sb.append(indexTable[(short) (s_fake << 6) >> 10 & 0x3f]);
				bb.rewind();
				s_fake = bb.getShort(1);
				sb.append(indexTable[(short) (s_fake << 4) >> 10 & 0x3f]);
				bb.rewind();
				sb.append(indexTable[bb.get(2) & 0x3f]);
				count += 4;
			}

			else if (bb.position() == 2) {
				if (count % LINECUT == 0 && count != 0)
					sb.append("\n");
				bb.put((byte) 0x00);
				bb.flip();
				s_fake = bb.getShort(); // used to get 2 bytes
				sb.append(indexTable[s_fake >> 10 & 0x3f]);
				sb.append(indexTable[(short) (s_fake << 6) >> 10 & 0x3f]);
				bb.rewind();
				s_fake = bb.getShort(1);
				sb.append(indexTable[(short) (s_fake << 4) >> 10 & 0x3f]);
				sb.append("=");
			}

			else if (bb.position() == 1) {
				if (count % LINECUT == 0 && count != 0)
					sb.append("\n");
				bb.put((byte) 0x00);
				bb.put((byte) 0x00);
				bb.flip();
				s_fake = bb.getShort(); // used to get 2 bytes
				sb.append(indexTable[s_fake >> 10 & 0x3f]);
				sb.append(indexTable[(short) (s_fake << 6) >> 10 & 0x3f]);
				sb.append("==");
			}

		}
		
		fc = new FileOutputStream("data.out").getChannel();
		fc.write(ByteBuffer.wrap(sb.toString().getBytes()));
		fc.close();
	}

	public static void main(String[] args) throws IOException {
		encode();
	}
}
