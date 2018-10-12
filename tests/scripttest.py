from birdeec import *
top_level('''
dim a as string
println(a)
{@
print("hello world")
@}
''')
process_top_level()
clear_compile_unit()

try:
	top_level('''
	println(a@#!@#@
	{@
	print("hello world")
	@}
	''')
	process_top_level()
	assert(False)
except TokenizerException:
	e=get_tokenizer_error()
	print(e.linenumber,e.pos,e.msg)
clear_compile_unit()

try:
	top_level('''
import hash_map:hash_map

dim map = new hash_map
	''')
	process_top_level()
	assert(False)
except CompileException:
	e=get_compile_error()
	print(e.linenumber,e.pos,e.msg)
clear_compile_unit()


