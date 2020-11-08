from aiohttp import web
import socketio
import asyncio


#creating the socket-server
sio = socketio.AsyncServer(async_mode='aiohttp')


app = web.Application()
sio.attach(app)


async def background_task():
    """Example of how to send server generated events to clients."""
    count = 0
    while True:
        await sio.sleep(10)
        count += 1
        await sio.emit('my_response', {'data': 'Server generated event'})


@sio.event
async def specific_event(sid, data):
    pass
#an event that happens(message or action)


@sio.event                                          
async def connect(sid, environ):
    await sio.emit('my_response', {'data': 'Connected', 'count': 0}, room=sid)
    print('connect', sid)
#connects client to server
    raise ConnectionRefusedError('authentication failed')
    

@sio.event
async def disconnect(sid):
    print('disconnect', sid)
#disconnects client from server


#sio.emit('specific_event', {'data': 'foobar'}) 
#sends message to all users

#sio.emit('specific_event', {'data': 'foobar'}, room = user_sid) 
#sends message to specific user


if __name__ =='__main__':
    sio.start_background_task(background_task)
    web.run_app(app, host='192.168.0.13', port=2520)
