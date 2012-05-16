import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

public class Mapping {
	private static Map<String, Integer> tagIntegerMap = new HashMap<String, Integer>();
	public static Vector<String> inputArray = new Vector<String>();

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		try {
			tagIntegerMap.put("CC",1);
			tagIntegerMap.put("CD",2);
			tagIntegerMap.put("DT",3);
			tagIntegerMap.put("EX",4);
			tagIntegerMap.put("FW",5);
			tagIntegerMap.put("IN",6);
			tagIntegerMap.put("JJ",7);
			tagIntegerMap.put("JJR",8);
			tagIntegerMap.put("JJS",9);
			tagIntegerMap.put("LS",10);
			
			tagIntegerMap.put("MD",11);
			tagIntegerMap.put("NN",12);
			tagIntegerMap.put("NNS",13);
			tagIntegerMap.put("NNP",14);
			tagIntegerMap.put("NNPS",15);
			tagIntegerMap.put("PDT",16);
			tagIntegerMap.put("POS",17);
			tagIntegerMap.put("PRP",18);
			tagIntegerMap.put("PRP$",19);
			tagIntegerMap.put("RB",20);
			
			tagIntegerMap.put("RBR",21);
			tagIntegerMap.put("RBS",22);
			tagIntegerMap.put("RP",23);
			tagIntegerMap.put("SYM",24);
			tagIntegerMap.put("TO",25);
			tagIntegerMap.put("UH",26);
			tagIntegerMap.put("VB",27);
			tagIntegerMap.put("VBD",28);
			tagIntegerMap.put("VBG",29);
			tagIntegerMap.put("VBN",30);

			tagIntegerMap.put("VBP",31);			
			tagIntegerMap.put("VBZ",32);
			tagIntegerMap.put("WDT",33);
			tagIntegerMap.put("WP",34);
			tagIntegerMap.put("WP$",35);
			tagIntegerMap.put("WRB",36);
			tagIntegerMap.put("$",37);
			tagIntegerMap.put("#",38);
			tagIntegerMap.put("''",39);
			tagIntegerMap.put("``",40);
			
			tagIntegerMap.put("(",41);
			tagIntegerMap.put(")",42);
			tagIntegerMap.put(",",43);
			tagIntegerMap.put(".",44);
			tagIntegerMap.put(":",45);
                        /*Iterator it = tagIntegerMap.entrySet().iterator();
                        while (it.hasNext()){
                           Map.Entry pairs = (Map.Entry)it.next();
                           tagIntegerMap.put((String)pairs.getValue(),(Integer)pairs.getKey());
                        }*/
			FileReader fr = new FileReader("pos.test.txt");
			BufferedReader br = new BufferedReader(fr);

			FileWriter fw = new FileWriter("delete");
			BufferedWriter bw = new BufferedWriter(fw);
			String myreadline;
			while (br.ready()) {
				myreadline = br.readLine();
				inputArray.add(myreadline);
			}
                        int totalline=0;
			for (int j = 0; j < inputArray.size(); j++) {
                                if(totalline==2000) break;
				String data = inputArray.get(j);
				if (data.trim().length() != 0) {
					String splitedData[] = data.split(" ");
                                        assert splitedData.length==2;
                                        //System.out.println(splitedData);
					//data = splitedData[0] + " |" + tagIntegerMap.get(Integer.parseInt(splitedData[1]));
					data = splitedData[0] + " |" + tagIntegerMap.get((String)splitedData[1]);
					bw.write(data);
					bw.newLine();
				} else{ totalline+=1;
					bw.newLine();
				}
			}
                        System.out.println(totalline);
			bw.flush();
			bw.close();
			br.close();
			fw.close();
			br.close();
			fr.close();

		} catch (Exception e) {
			System.out.println(e.toString());
		}
	}
}
