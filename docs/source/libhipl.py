'''
Hipl image class utility

P. Lewis, UCL/NCEO
1st April 2020

'''
import numpy as np

class Hipl():

    def __init__(self,data=None,imageName=None):
        '''
        Initialise header with numpy array (data)
        '''
        if data is not None:
            data = np.array(data)
            if data.ndim > 3:
                print('dataset dimensions must be <= 3: %d here'%data.ndim)

        # list order for header output 'seq_name'?'seq_desc'
        self.header_list = ('magic','orig_name','num_frame',\
                   'orig_date','rows','cols','bits_per_pixel',\
                   'bit_packing','pixel_format','seq_history',\
                   )
    
        ## The header ust start with magic
        ## and end with head_end
        self.magic = 'HIPS '
        self.head_end = "\n.\n"
        
        # translation for numpy types
        self.translator = {
            'ubyte':'PFBYTE',
            'uint8':'PFBYTE',
            'uint16':'PFSHORT',
            'uint32':'PFINT',
            'float32':'PFFLOAT'
        }
        
        self.hipl_code = {
            'PFBYTE':  0,
            'PFSHORT': 1,
            'PFINT':   2,
            'PFFLOAT': 3
        }
        
        if data is not None:
            try:
                hipl_type = self.translator[data.dtype.name]
            except:
                print('data type %s not accepted'%data.dtype)
                print(self.translator)
                return
        else:
            hipl_type = 'PFBYTE'
        

        # get data size from shape
        if data is not None:
            nrows,ncols,nframes = (data.shape + (1,1))[:3]
            itemsize = data.dtype.itemsize
        else:
            nrows,ncols,nframes = 0,0,0
            itemsize = 0
            
        from datetime import datetime
        # datetime object containing current date and time
        today = datetime.now().strftime("%b-%d-%Y %H:%M:%S")

        self.image = {
            'magic'          : self.magic,
            'num_frame'      : nframes,
            'orig_date'      : today,
            'rows'           : nrows,
            'cols'           : ncols,
            'bits_per_pixel' : 8*itemsize,
            'bit_packing'    : 0,
            'pixel_format'   : self.hipl_code[hipl_type],
        }

        self.data = data
       
        self._init_name(imageName)

    def write(self,filename=None):
        '''
        Write HIPL format header and data to 
        filename or self.image['name']
        '''
        image = self.image
        image['name'] = filename or image['name']
        
        self._update_name(image['name'])
        
        with open(image['name'], "wb") as of:
            
            self._write_head(of,image)
            
            # size is +3 for "\n.\n" we will add
            size = of.tell() + 3
            
            # need to be on 4 byte boundary
            extra = b" " * (4 - size%4)
            of.write(extra)
            of.write(self.head_end.encode('utf-8'))

            # write binary dataset
            self.data.tofile(of)
            # flush the buffer, in case we want to read
            of.flush()
        return

    def read(self,filename=None):
        '''
        read from Hipl format file into data and header
        '''
        
        filename = filename or self.image['name']
        
        '''
        First, find how long the header is
        '''
        buffer = ''
        
        with open(filename, "rb") as f:
            # read the 1st 4 bytes to check for magic
            byte = f.read(4)
            buffer += byte.decode('utf-8')
            if byte != self.magic[:4].encode('utf-8'):
                print('magic %s not found in image %s %s'%(self.magic,filename,str(byte)))
                
            # read next set of bytes to check for end of header mark
            while byte[1:] != self.head_end.encode('utf-8'):
                # Do stuff with byte.
                byte = f.read(4)
                buffer += byte.decode('utf-8')
            # this is the number of bytes of the header
            head_length = f.tell()
            #print(head_length)
            # split buffer into array
            buffer = buffer.split('\n')
            # load into header
            self._load_head_from_buffer(buffer)
            
            # sort pixel format
            pixel_format = int(self.image['pixel_format'])
            # find where hipl_code is pixel_format
            (keys,values) = zip(*self.hipl_code.items())
            hipl_code = np.array(keys)[np.array(values) == pixel_format][0]
            # find where translator is hipl_code
            (keys,values) = zip(*self.translator.items())
            # data type name
            dtname = np.array(keys)[np.array(values) == hipl_code][0]
            
            # read the rest
            buffer = f.read()
            
            # read the rest to buffer
            self.data = np.frombuffer(buffer, dtype=np.dtype(dtname))
            
            # sort the shape
            shape = int(self.image['rows']),int(self.image['cols'])
            nframes = int(self.image['num_frame'])
            if nframes > 1:
                shape += (nframes,)
    
            self.data = self.data.reshape(shape)
            
        return self.data
            
    def _init_name(self,imageName):
        '''
        Initialise header filename
        '''
        if imageName is None: return
    
        self.image['name'] = imageName
        self.image['orig_name'] = imageName
        self.image['seq_name']  = imageName
        self.image['seq_history']  = imageName
        self.image['seq_desc']  = imageName + ' generated from Hipl class'
        return

    def _update_name(self,imageName):
        '''
        Update header information with action
        '''
        if imageName is None: 
            return

        self._init_name(imageName)
        return
    
    def _load_head_from_buffer(self,of):
        '''
        load image HIPL header from list of
        '''
        end = len(self.header_list) - 1
        for i,item in enumerate(self.header_list):
            #print(item,':',i,':',of[i])
            self.image[item] = of[i]
        
    def _write_head(self,of,image):
        '''
        write image HIPL header to of
        '''
        end = len(self.header_list) - 1
        for i,item in enumerate(self.header_list):
            of.write(("%s"%str(self.image[item])).encode('utf-8'))
            # newline except at end
            if i != end: 
                of.write(("\n").encode('utf-8'))
        
def test1():
    from tempfile import NamedTemporaryFile
    f = NamedTemporaryFile(delete=True)
    
    for dtype in (np.ubyte, np.uint8, np.uint16, np.float32):
        # generate a test image
        array_in = np.array([[3,2,3,4],[4,5,6,7],[7,8,9,100]]).astype(dtype)
        self = Hipl(array_in)

        self.write(f.name)
        array_out = self.read(f.name)

        # are they the same?
        print ('pass',np.dtype(dtype).name, (array_out-array_in).all() == 0)

def test2():
    import pylab as plt
    f = '../start/tests/dem_1/rabbit.hips'
    rabbit=Hipl().read(f)
    #print(rabbit)
    plt.imshow(rabbit)
    plt.colorbar()
    plt.show()
    
def test3():
    import pylab as plt
    f = '../start/tests/dem_1/smoothyd.hips'
    rabbit=Hipl().read(f)
    #print(rabbit)
    plt.imshow(rabbit)
    plt.colorbar()
    plt.show()
    
