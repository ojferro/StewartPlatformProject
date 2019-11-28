import zmq

def send_error(socket, frame_num, cv_err, flags=0):
    md = dict(
        frame = frame_num,
        err = cv_err
    )
    return socket.send_json(md, flags)

def recv_error(socket, flags=0):
    md = socket.recv_json(flags=flags)
    return [md['frame']] + md['err']

def create_socket(socket_type = zmq.SUB):
    context = zmq.Context()
    socket = context.socket(socket_type)

    if (socket_type == zmq.PUB):
        socket.bind("tcp://127.0.0.1:5556")
    else:
        socket.setsockopt(zmq.SUBSCRIBE, b"")
        socket.connect("tcp://127.0.0.1:5556")
    
    return socket
