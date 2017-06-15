import json
import sys
import helping_functions

EOL = "\n"
EOS = ";"
SPACE = " "
EQUAL = " = "
BODY_BEGIN = "{"
BODY_END = "}"
ARRAY_BEGIN = "["
ARRAY_END = "]"
TDEF = "typedef"

def generate(generated_file_name="config.h"):
	paraJS = open("parameter1.json", "r")
	json_data = json.load(paraJS)
	arr = helping_functions.read_params(sys.argv[1])

	str1 = ""
	str1 += generate_import(json_data["import"])

	t1 = raw_input("Please enter the type of input: ")
	t2 = raw_input("Please enter the type of weights: ")
	t3 = raw_input("Please enter the type of output: ")
	arr1 = [t1, t2, t3]
	str1 += generate_type_definition(json_data["type_definition"], arr1)
	str1 += generate_preprocessor(json_data["preprocessor"])
	str1 += generate_var(json_data["var"])	
	str1 += generate_body(json_data["params"], arr)
	str1 += generate_end(json_data["end"])
	print str1
	with open("../example/test_demo/inference_net/" + generated_file_name, "w") as generated_file:
        	generated_file.write(str1)
	return str1


def generate_import(import_json):
	import_str = EOL
	for import_sen in import_json:
        	import_str += import_sen + EOL
	return import_str

def generate_type_definition(type_json, arr):
	type_str = EOL * 2 
	type_str += "//define data type" + EOL
	for type_sen, t in zip(type_json, arr):
        	type_str += TDEF + SPACE + t + SPACE + type_sen + EOS + EOL
	return type_str

def generate_preprocessor(prep_json):
	prep_str = EOL * 2
	prep_str += "// C++ compilation debug mode" + EOL
	for prep_sen in prep_json:
        	prep_str += prep_sen + EOL
	return prep_str

def generate_var(var_json):
	var_str = EOL * 2
	var_str += "//network configuration PARAMETERS" + EOL
	for var_sen in var_json:
        	var_str += var_sen["type"] + SPACE + var_sen["name"] + EQUAL + var_sen["value"] + EOS + EOL
	return var_str
	
def generate_body(body_json, arr):
	body_str = EOL * 2
	
	for body_sen in body_json:
		if body_sen["name"] in arr:
			
			data = arr[arr.index(body_sen["name"])+1] 
			arr1 = data.split()
			
			if body_sen["name"] == 	"nn_in_number_conv":
				data1 = arr[arr.index("nn_group_conv")+1] 
				arr2 = data1.split()
				for i in range(len(arr1)):
					print(i)
					print("ooo")
					arr1[i] = str(int(arr1[i])/int(arr2[i]))			
			
		
        		body_str += body_sen["type"] + SPACE + body_sen["name"] + ARRAY_BEGIN + str(len(arr1)) + ARRAY_END + EQUAL + BODY_BEGIN + ', '.join(arr1) + BODY_END + EOS + 				EOL
	body_str += EOL
	
	return body_str

def generate_end(end_json):

   end_str = EOL
   for e in end_json:
	end_str += e + EOL
   return end_str

if __name__ == "__main__":
    generate()
