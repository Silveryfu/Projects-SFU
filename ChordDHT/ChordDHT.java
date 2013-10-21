import java.io.*;
import java.nio.*;
import java.nio.channels.FileChannel;
import java.util.*;

public class ChordDHT {

	private static ChordDHT my_chord = null;
	private int s; // hash space size==s+1
	private int n; // number of nodes
	private int m; // number of keys
	private Map<Integer, dht_node> nodesMap = new HashMap<Integer, dht_node>();

	private StringBuilder sb = new StringBuilder();

	private class dht_node {
		private int id;
		public Set<Integer> si = new HashSet<Integer>();
		/* somehow i wanna use Map...dalala */
		Map<Integer, dht_node> key_map = new HashMap<Integer, dht_node>();

		dht_node(int a) {
			id = a;
		}

		public void addFile(int i) {
			si.add(i);
		}

		public int getId() {
			return id;
		}

		public void updateMap() {
			int finger = 1;
			int flag;
			SortedSet<Integer> keys = new TreeSet<Integer>(nodesMap.keySet());
			while (finger < (s + 1)) {
				flag = 0;
				for (Integer key : keys) {
					if (key >= ((id + finger) % (s + 1))) { // if the node is
															// just >= than the
															// key
						key_map.put((id + finger) % (s + 1), nodesMap.get(key));
						flag = 1;
						break;
					}
				}
				if (flag == 0)
					key_map.put((id + finger) % (s + 1),
							nodesMap.get(keys.first())); // the key is store in
															// the first node
				finger *= 2;
			}
		}

		public void queryDHT(Integer key_to_find) throws IOException {
			if (si.contains(key_to_find)) {
				writeFile(id, 1);
				return;
			}
			SortedSet<Integer> keys = new TreeSet<Integer>(key_map.keySet());
			writeFile(id, 0);
			if (key_to_find < keys.first())
				key_map.get(keys.last()).queryDHT(key_to_find);   //if the key is less than the least node key go to the largest one
			else {
				key_map.get(keys.subSet(0, key_to_find + 1).last()).queryDHT(
						key_to_find); // left inclusive, right exclusive, so + 1
			}
		}

		public void printTable() { // for unit test
			Iterator it = key_map.entrySet().iterator();
			System.out.println("id is" + id + this);
			while (it.hasNext()) {
				System.out.println(it.next());
			}
			System.out.println("\n");
		}
	}

	/* end of dht_node class */

	public void queryDHT(int key, dht_node node) throws IOException {
		node.queryDHT(key);
	}

	public void writeFile(Integer id, int signal) throws IOException {
		sb.append(id + " ");
		if (signal == 1) {
			sb.append("\n");
			FileChannel fc = new FileOutputStream("./dht.out").getChannel();
			fc.write(ByteBuffer.wrap(sb.toString().getBytes()));
			fc.close();
		}
	}

	public void readFile() throws IOException {
		String tmp;
		String[] entity;
		int count = 0;
		@SuppressWarnings("resource")
		BufferedReader in = new BufferedReader(new FileReader("./dht.in"));
		tmp = in.readLine();
		while (tmp != null) {
			entity = tmp.split(",");
			for (String str : entity) {
				switch (count) {
				case 0:
					s = Integer.valueOf(str);
					break;
				case 1:
					n = Integer.valueOf(str);
					break;
				case 2:
					m = Integer.valueOf(str);
					break;
				case 3:
					int id = Integer.valueOf(str);
					dht_node d = new dht_node(id);
					nodesMap.put(id, d);
					SortedSet<Integer> keys = new TreeSet<Integer>(
							nodesMap.keySet());
					for (Integer key : keys) {
						nodesMap.get(key).updateMap();
					}
					break;
				case 4:
					int k = Integer.valueOf(str);
					SortedSet<Integer> keys2 = new TreeSet<Integer>(
							nodesMap.keySet());
					if (k > keys2.last()) {
						nodesMap.get(keys2.first()).addFile(k);
					} else
						nodesMap.get(keys2.tailSet(k).first()).addFile(k);
					queryDHT(k, nodesMap.get(3)); // always start from node 1 as
													// required
					break;
				}
			}
			count++;
			tmp = in.readLine();
		}
	}

	public static ChordDHT getInstance() {
		if (my_chord == null) {
			my_chord = new ChordDHT();
		}
		return my_chord;
	}

	private ChordDHT() {
	};

	public static void main(String[] args) throws IOException {
		ChordDHT cd = ChordDHT.getInstance();
		cd.readFile();
	}
}
