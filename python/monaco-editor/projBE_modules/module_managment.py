import module
import MonacoEditorBE

class ModuleMS:
	def __init__(self):
		self.modules = {}
		self.func_list = {}

	def run(self, module_id, req, module_request, sdata_protocol):
		return self.modules[module_id].RUN(req, module_request, sdata_protocol)


	def module_instantiate(self, func_list):
		self.func_list = func_list
		#instantiate module objects

		#vpt_module = VPT_BE.VPTModule("VPT", self.func_list)
		meditor = MonacoEditorBE.MEditor("meditor", self.func_list)

		#instantiate module objects

		#self.modules[vpt_module.Get().id] = vpt_module
		self.modules[meditor.Get().id] = meditor


#def functest(args):
	#return "test"

#func_list = {"testfunc": functest}
#module_ms = ModuleMS()

#module_ms.module_instantiate(func_list)
