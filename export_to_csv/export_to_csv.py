__author__ = 'scott'

import csv
import json

# This python script takes the data that was outputted
# by AWS pipeline from a DynamoDB and parses the data into a CSV file.


# This is used to convert unicode json data into ASCII
def byteify(input):
    if isinstance(input, dict):
        return {byteify(key): byteify(value)
                for key, value in input.iteritems()}
    elif isinstance(input, list):
        return [byteify(element) for element in input]
    elif isinstance(input, unicode):
        return input.encode('utf-8')
    else:
        return input


myList = []
# myValues = [] #this didn't work in the end

myfile = open('exported_data','r')

for idx, line in enumerate(myfile):

    padless = line.rstrip()
    # myValues.append(padless) #this didn't work in the end

    # Create a JSON list
    jsonData = json.loads(padless)
    jsonData = byteify(jsonData)
    myList.append(jsonData)


for myDict in myList:
    for k,v in myDict.iteritems():
        for val in v.values():
            print val
            print v.values()
            # myDict[k]=v.values()
            myDict[k]=val


print myList
# print myValues #this didn't work in the end

myfile.close()

# TODO: Generalize so that it adds ALL the possible keys, not just the ones in the first item in case the keys are not uniform
keys = myList[1].keys()

with open('finalformat.csv', 'wb') as output_file:
    dict_writer = csv.DictWriter(output_file, keys)
    dict_writer.writeheader()
    dict_writer.writerows(myList)



# Here was try number 1, parsing directly from myfile
# For some really off reason, this loop doesn't work the second time around...
# for idx, line in enumerate(myfile):
#
#     #let's remove the padding on our data
#     padless = line.rstrip()
#     print(padless)
#     # here is what the JSON data looked after loading the stripped line
#     jsonData = json.loads(padless)
#     # print(json.dumps(jsonData))
#
#     #if we append the jsonData
#     myList.append(jsonData)

# Trying contents, but this seems to create a character array each of which gets stored as a seperate list item
# contents = myfile.read()
# print(contents)

# Just taking out the 's
# for item in myList:
#     item.replace("'","")

# Using encode to get rif of the u'
# for key, value in myList.iteritems():
#     key.encode('ascii')
#     value.encode('ascii')


# Using a byteify function to try and re-encode the data
# def json_load_byteified(file_handle):
#     return _byteify(
#         json.load(file_handle, object_hook=_byteify),
#         ignore_dicts=True
#     )
#
# def json_loads_byteified(json_text):
#     return _byteify(
#         json.loads(json_text, object_hook=_byteify),
#         ignore_dicts=True
#     )
#
# def _byteify(data, ignore_dicts = False):
#     # if this is a unicode string, return its string representation
#     if isinstance(data, unicode):
#         return data.encode('utf-8')
#     # if this is a list of values, return list of byteified values
#     if isinstance(data, list):
#         return [ _byteify(item, ignore_dicts=True) for item in data ]
#     # if this is a dictionary, return dictionary of byteified keys and values
#     # but only if we haven't already byteified it
#     if isinstance(data, dict) and not ignore_dicts:
#         return {
#             _byteify(key, ignore_dicts=True): _byteify(value, ignore_dicts=True)
#             for key, value in data.iteritems()
#         }
#     # if it's anything else, return it in its original form
#     return data