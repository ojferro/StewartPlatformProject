import zmq

def send_error(socket, frame_num, cv_err, flags=0, copy=True, track=False):
    md = dict(
        frame = frame_num,
        err = cv_err
    )
    return socket.send_json(md, flags)

def recv_error(socket, flags=0, copy=True, track=False):
    md = socket.recv_json(flags=flags)
    print("Received error data from frame {}, data: {}".format(md['frame'], md['err']))
    return md['err']

def create_socket(socket_type = zmq.SUB):
    context = zmq.Context()
    socket = context.socket(socket_type)

    if (socket_type == zmq.PUB):
        socket.bind("tcp://*:5555")
    else:
        socket.connect("tcp://localhost:5555")
    
    return socket