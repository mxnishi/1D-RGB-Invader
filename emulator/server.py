#!/usr/bin/env python3
"""Local-only bridge to a native build of the original Arduino sketch."""
import atexit,json,os,pathlib,re,subprocess,threading,urllib.parse
from http.server import ThreadingHTTPServer,BaseHTTPRequestHandler
ROOT=pathlib.Path(__file__).resolve().parent
PORT=int(os.environ.get('RGB_EMULATOR_PORT','8766'))
class Machine:
    def __init__(self):self.lock=threading.Lock();self.boot()
    def boot(self):
        for _ in range(3):
            self.proc=subprocess.Popen([str(ROOT/'emulator'),str(ROOT/'preferences.txt')],stdin=subprocess.PIPE,stdout=subprocess.PIPE,text=True,bufsize=1)
            line=self.proc.stdout.readline()
            if line:self.state=json.loads(line);return
            self.proc.wait(timeout=3)
        raise RuntimeError('Firmware did not boot')
    def request(self,line):
        with self.lock:
            self.proc.stdin.write(line+'\n');self.proc.stdin.flush()
            result=self.proc.stdout.readline()
            if not result:raise RuntimeError('Emulator process exited unexpectedly')
            self.state=json.loads(result)
            answer=self.state
            if answer['restart']:
                self.proc.wait(timeout=3);self.boot()
            return answer
    def close(self):
        if self.proc.poll() is None:self.proc.terminate();self.proc.wait(timeout=3)
if not (ROOT/'emulator').exists():subprocess.run(['python3',str(ROOT/'build.py')],check=True)
machine=Machine();atexit.register(machine.close)
def hx(s):return str(s).encode().hex() or '-'
class Handler(BaseHTTPRequestHandler):
    def log_message(self,*args):pass
    def reply(self,status,data,kind='application/json',headers=None):
        if isinstance(data,(dict,list)):data=json.dumps(data)
        if isinstance(data,str):data=data.encode()
        self.send_response(status);self.send_header('Content-Type',kind+'; charset=utf-8');self.send_header('Cache-Control','no-store');self.send_header('X-Content-Type-Options','nosniff')
        for k,v in (headers or {}).items():self.send_header(k,v)
        self.send_header('Content-Length',str(len(data)));self.end_headers();self.wfile.write(data)
    def do_GET(self):
        path=urllib.parse.urlsplit(self.path).path
        if path in ('/','/index.html'):return self.reply(200,(ROOT/'index.html').read_bytes(),'text/html')
        if path=='/api/state':return self.reply(200,machine.request('tick 0 0'))
        self.firmware(path,{k:v[-1] for k,v in urllib.parse.parse_qs(urllib.parse.urlsplit(self.path).query,keep_blank_values=True).items()})
    def do_POST(self):
        origin=self.headers.get('Origin','')
        if origin and origin not in (f'http://127.0.0.1:{PORT}',f'http://localhost:{PORT}'):return self.reply(403,{'error':'Local origin required'})
        try:
            length=int(self.headers.get('Content-Length',0))
            if length>65536:return self.reply(413,{'error':'Request too large'})
            raw=self.rfile.read(length)
            path=urllib.parse.urlsplit(self.path).path
            if path.startswith('/api/'):
                data=json.loads(raw or '{}')
                if path=='/api/tick':return self.reply(200,machine.request(f"tick {max(0,min(100,int(data.get('ms',0))))} {int(data.get('mask',0))&15}"))
                if path=='/api/scene':
                    scene=int(data.get('scene',1))
                    if scene not in (*range(1,12),100,101,102):raise ValueError('Unknown scene')
                    return self.reply(200,machine.request(f'scene {scene}'))
                return self.reply(404,{'error':'Unknown command'})
            self.firmware(path,{k:v[-1] for k,v in urllib.parse.parse_qs(raw.decode(),keep_blank_values=True).items()})
        except (ValueError,KeyError,TypeError) as e:self.reply(400,{'error':str(e)})
        except Exception as e:self.reply(500,{'error':str(e)})
    def firmware(self,path,args):
        allowed={'/training','/settings','/save','/loadprofile','/reset','/sounds','/savesounds','/colors','/savecolors','/admin/stats'}
        if path=='/update':return self.reply(501,'OTA flashing is not emulated. Rebuild with build.py to run a different sketch.','text/plain')
        if path not in allowed:return self.reply(404,'Not found','text/plain')
        # The hardware handler trusts its forms. Bound host inputs before calling it.
        if path=='/save':
            ranges={'leds':(24,1100),'bright':(10,100),'startlvl':(1,11),'hb_size':(1,5),'shot_spd':(50,150),'vol':(0,100)}
            for i in range(1,11):ranges.update({f'lspd{i}':(1,100),f'llen{i}':(1,300),f'lboss{i}':(0,3)})
            for b in (1,2,3):
                for suffix,limits in {'mv':(1,100),'ss':(1,150),'hp':(1,30),'fr':(1,200),'bc':(1,30),'m1':(5,95),'m2':(5,95),'m3':(5,95)}.items():ranges[f'b{b}{suffix}']=limits
            required={'leds','bright','startlvl','hb_size','shot_spd','vol',*(f'{p}{i}' for i in range(1,11) for p in ('lspd','llen','lboss')), 'b1mv','b1ss','b1hp','b1fr','b2mv','b2ss','b2hp','b2fr','b2m1','b2m2','b2m3','b3mv','b3hp','b3fr','b3bc'}
            if not required<=args.keys():return self.reply(400,'Save the complete settings form.','text/plain')
            try:
                for key,(lo,hi) in ranges.items():
                    if key in args and not lo<=int(args[key])<=hi:raise ValueError(f'{key} must be between {lo} and {hi}')
                marks=[int(args['b2m1']),int(args['b2m2']),int(args['b2m3'])]
                if not marks[0]>marks[1]>marks[2]:raise ValueError('Boss markers must descend: Mark 1 > Mark 2 > Mark 3')
            except ValueError as e:return self.reply(400,str(e),'text/plain')
        line='http '+hx('/' if path=='/settings' else path)+' '+str(len(args))
        for k,v in args.items():line+=' '+hx(k)+' '+hx(v)
        state=machine.request(line);response=state['http'];body=bytes.fromhex(response['body']).decode();kind=response['type']
        if kind=='text/html':
            body=body.replace("href='/'","href='/settings'").replace('href="/"','href="/settings"')
            body=re.sub(r"<div class='sub-head'>.*?</div>", '', body, flags=re.DOTALL)
            # Omit the donation footer from the emulator presentation only.
            body=re.sub(r"<div class='credits'><a href='https://paypal\.me/WeisWernau'.*?</div>", '', body, flags=re.DOTALL)
        headers={}
        if response['location']:headers['Location']=bytes.fromhex(response['location']).decode().replace('/','/settings',1) if bytes.fromhex(response['location']).decode()=='/' else bytes.fromhex(response['location']).decode()
        return self.reply(response['code'],body,kind,headers)
print(f'Firmware emulator: http://127.0.0.1:{PORT}',flush=True)
ThreadingHTTPServer(('127.0.0.1',PORT),Handler).serve_forever()
