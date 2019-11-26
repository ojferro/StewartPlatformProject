import zmq

def send_error(socket, frame_num, cv_err, flags=0, copy=True, track=False):
    md = dict(
        frame = frame_num,
        err = cv_err
    )
    print("Sending data: {}".format(md))
    return socket.send_json(md, flags)

def recv_error(socket, flags=0, copy=True, track=False):
    md = socket.recv_json(flags=flags)
    print("Received error data from frame {}, data: {}".format(md['frame'], md['err']))
    return md['err']

def create_socket(socket_type = zmq.SUB, eth_addr='127.0.0.1'):
    context = zmq.Context()
    socket = context.socket(socket_type)

    if (socket_type == zmq.PUB):
        socket.bind("tcp://*:5555")
        print("Bound socket to {}".format("tcp://*:5555"))
    else:
        socket.connect("tcp://%s:5555" % eth_addr)
        print("Bound socket to {}".format("tcp://%s:5555" % eth_addr))
    
    return socket