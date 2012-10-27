blppy
=====

A python binding for Bloomberg's BLPAPI - sprinkle some python awesomeness on your Bloomberg - an unofficial, and otherwise absent, Python binding for the 
BLPAPI SDK, available at www.openbloomberg.com.

As simple as,

import blp

session = blp.Session('localhost', 8194)
session.openService('//blp/refdata')
service = session.getService('//blp/refdata')
request = service.createRequest('HistoricalDataRequest') \
	.addSecurity('GOOG US Equity') \
        .addField('PX_LAST') \
        .set('startDate', '20080101') \
        .set('endDate', '20120601')
session.sendRequest(request)
response = session.getResponse()

print str(response)

Enjoy.

Kenvor Cothey (kenvor@heaven.li)
