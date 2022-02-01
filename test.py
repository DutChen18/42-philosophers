import asyncio
import sys
import time

def progress(p, data):
	data.append("\033[37m[")
	for i in range(20):
		if i < p * 20:
			data.append(f"\033[97m=")
		else:
			data.append(f"\033[90m=")
	data.append("\033[37m]\033[0m")

class Philo:
	def __init__(self, prog, index):
		self.prog = prog
		self.state = "THINK"
		self.index = index
		self.t_eat = 0
		self.t_sleep = 0

	def write(self, data):
		data.append(f"\033[37m{self.index + 1:03}\033[0m ")
		if self.state == "THINK":
			data.append(f"\033[36mTHINK\033[0m ")
		if self.state == "EAT":
			data.append(f"\033[32m EAT \033[0m ")
		if self.state == "SLEEP":
			data.append(f"\033[34mSLEEP\033[0m ")
		if self.state == "DEATH":
			data.append(f"\033[31mDEATH\033[0m ")
		progress((self.prog.time - self.t_eat) / self.prog.t_die, data)
		if self.state == "EAT":
			data.append(f" ")
			progress((self.prog.time - self.t_eat) / self.prog.t_eat, data)
		if self.state == "SLEEP":
			data.append(f" ")
			progress((self.prog.time - self.t_sleep) / self.prog.t_sleep, data)

class Prog:
	def __init__(self, count, t_die, t_eat, t_sleep):
		self.count = count
		self.t_die = t_die
		self.t_eat = t_eat
		self.t_sleep = t_sleep
		self.philos = [Philo(self, index) for index in range(count)]

	async def start(self, program):
		self.proc = await asyncio.create_subprocess_exec(program,
			str(self.count), str(self.t_die), str(self.t_eat), str(self.t_sleep),
			stdout=asyncio.subprocess.PIPE)
		self.start = int(time.time() * 1000)
		self.time = 0
		self.running = True
	
	async def monitor(self):
		while self.running:
			await asyncio.sleep(0.01)
			self.time = int(time.time() * 1000) - self.start
			data = []
			data.append("\033[H")
			for philo in self.philos:
				philo.write(data)
				data.append("\033[K\n")
			data.append("\033[J")
			sys.stdout.write("".join(data))
			sys.stdout.flush()

	async def run(self):
		while True:
			line = await self.proc.stdout.readline()
			if line == b"":
				break
			t, n, *s = line.decode().split(" ")
			t = int(t)
			n = int(n) - 1
			s = " ".join(s)[:-1]
			if s == "is thinking":
				self.philos[n].state = "THINK"
			if s == "is eating":
				self.philos[n].state = "EAT"
				self.philos[n].t_eat = t
			if s == "is sleeping":
				self.philos[n].state = "SLEEP"
				self.philos[n].t_sleep = t
			if s == "died":
				self.philos[n].state = "DEATH"
		self.running = False

async def main(argv):
	count = int(argv[1])
	t_die = int(argv[2])
	t_eat = int(argv[3])
	t_sleep = int(argv[4])

	prog = Prog(count, t_die, t_eat, t_sleep)
	await prog.start("philo/philo")
	await asyncio.gather(prog.monitor(), prog.run())

if __name__ == "__main__":
	asyncio.run(main(sys.argv))
