#!/usr/bin/python 
#
# @package
# Python functions used by the islandora_irods module in order to access the iRODS system.
#

import sys
from irods.session import iRODSSession
from irods.models import Collection, DataObject, DataObjectMeta


## Retrieve Metadata from iRODS object.
#
# @param String id
#   id of the related irods data object
# 
def getMetadata(id, host, port, user, password, zone):
    dataObject = getDataObject(id, host, port, user, password, zone)
    
    metadata = dataObject.metadata.keys()

    # construct table for display
    mdTable = '<table>'
    for m in metadata:
        for v in dataObject.metadata.get_all(m):
            # name and unit in first column
            mdTable += '<tr><td>' + m 
            if v.units is not None:
                mdTable += ' (' + v.units + ')' 
            mdTable += '</td>' + '<td>'
            # value in second column
            mdTable += v.value 
            mdTable += '<br/>'
            mdTable += '</td></tr>'
    mdTable += '</table>'

    print mdTable.encode("utf-8","backslashreplace")


## Retrieve iRODS object as file and print it as string.
#
# @param String id
#   id of the related irods data object
# @param String/Integer offset
#   file seek offset position (offset points to the first byte to be read; start counting from 1 for the first byte!)
# @param String/Integer amount
#   amount of bytes to be read
# 
def getFile(id, host, port, user, password, zone, offset=0, amount=0):
    dataObject = getDataObject(id, host, port, user, password, zone)
    with dataObject.open() as f:
        f.seek( int(offset), 0)
        print f.read( int(amount) )


## Get File Size of iRODS-object.
#
# @param String id
#   id of the irods data object
#
def getFileSize(id, host, port, user, password, zone):
    dataObject = getDataObject(id, host, port, user, password, zone)    
    print dataObject.size

## Get filename of iRODS-object.
#
# @param String id
#   id of the irods data object
#
def getFileName(id, host, port, user, password, zone):
    dataObject = getDataObject(id, host, port, user, password, zone)    
    print dataObject.name


## Check if data object existent in iRODS.
#
# @param String id
#   id of the irods data object
# 
def checkForDataObject(id, host, port, user, password, zone):
    if getDataObject(id, host, port, user, password, zone) != None:
        print "exists"


## Retrieve data object with type="AIP" and uuid=id from iRODS.
#
# @param String id
#   id of the irods data object
# @param String host
#   host of the icat server
# @param String port
#   port of the icat server
# @param String user
#   irods user
# @param String password
#   irods password
# @param String zone
#   irods zone
# @return DataObject
#   object if found, else: None 
# 
def getDataObject(id, host, port, user, password, zone):
    sess = iRODSSession(host=host, port=port, user=user, password=password, zone=zone)

    # query for all elements with specified uuid
    queryResult = sess.query(Collection.name, DataObject.name, DataObjectMeta.name, DataObjectMeta.value).filter( DataObjectMeta.name == "uuid", DataObjectMeta.value == id ).all()
    #if queryResult is not None:
        #print "found", len(queryResult)
        #print queryResult
    # test for and return element with type="AIP"
    for element in queryResult:
        testobj = sess.data_objects.get(element[Collection.name] + "/" + element[DataObject.name])
        for type in testobj.metadata.get_all('type'):
            if "AIP" in type.value:
                #collectionName = element[Collection.name]
                #dataObjectName = element[DataObject.name]
                #print collectionName + "/" + dataObjectName
                return testobj
    
    return None
