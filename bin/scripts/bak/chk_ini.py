from ConfigParser import ConfigParser
import sys, os.path, traceback

if __name__ == '__main__':
	try:
		if len(sys.argv) < 3:
			sys.exit('usage:\n %s to_check_ini vaild_options' % os.path.basename(sys.argv[0]))

		chk = sys.argv[1]
		ref = sys.argv[2]
		
		chk_ini = ConfigParser()
		chk_ini.optionxform = lambda x: x
		chk_ini.read(chk)

		ref_ini = ConfigParser()
		ref_ini.optionxform = lambda x: x
		ref_ini.read(ref)

		print >>sys.stderr, 'Checking "%s" against reference ini "%s" ... ' % (chk, ref),

		for s in ref_ini.sections():
			for o in ref_ini.options(s):
				_v1 = ref_ini.get(s, o, raw = True)
				_v2 = chk_ini.get(s, o, raw = True)
				if _v1 != _v2:
					sys.exit('Unexpected value of "%s.%s": "%s"(expected"%s")' %(s, o, _v2, _v1))

		print >>sys.stderr, 'ok'
	except SystemExit:
		print >>sys.stderr, sys.exc_info()[1] 
		sys.exit(3)
	except:
		sys.exit(traceback.format_exc ())

