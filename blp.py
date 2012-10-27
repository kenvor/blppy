import blpapi

# TODO: add event types

class Session():
    def __init__(self, host, port):
        self.create(host, port)
        
    def create(self, host, port):
        self._session = blpapi.Session(host, port)
        self._session.start()        
        self.purge()
        
    def openService(self, service_name):
        self._session.openService(service_name)
        
    def getService(self, service_name):
        service = Service(self._session.getService(service_name))
        self.purge()
        return service

    def purge(self):
        event = self._session.tryNextEvent()
        while event != None:
            event = self._session.tryNextEvent()
            
    def sendRequest(self, request):
        self._session.sendRequest(request._request)
        
    def getResponse(self):
        r = []
        event = self._session.nextEvent() # block
        while event != None:
            msg = event.nextMessage()
            while msg != None:
                r.append(self.parseMsg(msg))
                msg = event.nextMessage()

            event = self._session.nextEvent(1000) # TODO: check for "last event" event to break

        return r        
    
    def parseMsg(self, msg):
        e = Element(msg.asElement())
        return e.parse()
 

class Service():
    def __init__(self, service):
        self._service = service
        
    def createRequest(self, request_type):
        return Request(self._service.createRequest(request_type))

class Request():
    def __init__(self, request):
        self._request = request
        
        # TODO: build request from dict?
        
    def set(self, key, value):
        self._request.set(key, value)
        return self
        
    def addOverride(self, **kwargs):
        # TODO: only works for certains types of request, filter here?
        overrides = self._request.getElement('overrides')
        for k in kwargs.keys():
            o = overrides.appendElement()
            o.setElement('fieldId', k)
            o.setElement('value', kwargs[k])

        return self
    
    def addSecurity(self, security):
        self._request.getElement('securities').appendValue(security)
        return self
    
    def addField(self, field):
        self._request.getElement('fields').appendValue(field)
        return self
    
    def parse(self):
        return Element(self._request.asElement()).parse()
    
class Element():
    def __init__(self, element):
        self._element = element
        
    def unpackElement(self, e, o = 0):
        # print " " * o + e.name() + " " + str(e.datatype()) + " " + str(e.numElements()) + " " + str(e.numValues())
        if e.datatype() in [15, 16]:
            # TODO: what if an element has elements > 1 and values > 1?
            if e.numElements() > 0:
                t = {}
                for i in range(e.numElements()):
                    s = e.getElement(i)
                    t[s.name()] = self.unpackElement(s, o + 1)
            elif e.numValues() > 0:
                t = []
                for i in range(e.numValues()):
                    s = e.getValueAsElement(i)
                    t.append(self.unpackElement(s, o + 1))
            else: # nothing here?
                t = None
            r = t
        elif e.datatype() in range(1, 14):
            # TODO: how to tell the difference between a single element list and a scalar?
            if e.numValues() == 1:
                r = e.getValueAsString()
            elif e.numValues() > 1:
                t = []
                for i in range(e.numValues()):
                    t.append(e.getValueAsString(i))
                r = t
            else:
                r = None
        else: # unknown/unmanageable Element type
            r = None

        return r
    
    def parse(self):
        r = { self._element.name(): self.unpackElement(self._element) }
        return r

# TODO: deprecate
class Blp():
    def __init__(self):
        self.createSession()
    
    def createSession(self):
        self._session = Session('localhost', 8194)

    def openService(self):
        self._session.openService('//blp/refdata')
        return self._session.getService('//blp/refdata')
        
    def createRequest(self, service):
        return service.createRequest('HistoricalDataRequest')

    def addOverride(self, request, **kwargs):
        return request.addOverride(**kwargs)
    
    def addSecurity(self, request, security):
        return request.addSecurity(security)
    
    def addField(self, request, field):
        return request.addField(field)
    
    def sendRequest(self, request):
        self._session.sendRequest(request)
        
    def getResponse(self):
        return self._session.getResponse()
        
