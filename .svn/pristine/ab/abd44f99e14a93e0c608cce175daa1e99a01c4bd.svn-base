#! /bin/sh

import os, sys, re

if __name__ == '__main__' :
   if len(sys.argv) != 3 :
      print "Usage: $0 <view_name> <target dir>"
      print " eg: ngupta_tucl_lksctp_prototype ~/tmp"
      print "<target dir> shall be present already"
      sys.exit(1)
   else :
      # Get view and local directory path
      viewName = sys.argv[1]
      locaPath = sys.argv[2]

      intgListPath = '/vobs/core_prd/voip/tucl/src/intg'
      tuclPath = '/vobs/core_prd/voip/tucl/src/'
      sctpPath = '/u/prd/rls/1000191/1.2/crnt'
      commonPath = '/vobs/core_prd/cm/'

      print r'/bin/rm -rf ' + locaPath + '/*'
      os.system(r'/bin/rm -rf ' + locaPath + '/*')

      # Create directory elements
      #for elm in dirList :
      #   print 'Creating Directory ' + elm
      #   os.system('mkdir ' + elm)

      #print 'Creating Directory ' + 'hi'
      #os.system('mkdir ' + 'hi')


      # Link files from vob
      #for elm in dirList :
      #   print 'Linking File ' + elm
      #   print 'ln -s  /view/' + viewName + intgListPath + '/' + elm + '/* ' + locaPath + '/' + elm + '/. 2>/dev/null'
      #   os.system('ln -s  /view/' + viewName + intgListPath + '/' + elm + '/* ' + locaPath + '/' + elm + '/. 2>/dev/null')

      # Link tucl files from vob
      print 'Linking File ' + 'tl'
      print 'ln -s  /view/' + viewName + tuclPath + 'tl/' + '/* ' + locaPath + '/. 2>/dev/null'
      os.system('ln -s  /view/' + viewName + tuclPath + 'tl/' + '/* ' + locaPath + '/. 2>/dev/null')

      # Link tucl files from vob
      print 'Linking File ' + 'hi'
      print 'ln -s  /view/' + viewName + tuclPath + 'hi/' + '/* ' + locaPath + '/. 2>/dev/null'
      os.system('ln -s  /view/' + viewName + tuclPath + 'hi/' + '/* ' + locaPath + '/. 2>/dev/null')

      # Link tucl Acc files from vob
      print 'Linking File ' + 'acc'
      print 'ln -s  /view/' + viewName + tuclPath + 'acc/' + '/* ' + locaPath + '/. 2>/dev/null'
      os.system('ln -s  /view/' + viewName + tuclPath + 'acc/' + '/* ' + locaPath + '/. 2>/dev/null')

      # Link tucl Build files from vob
      print 'Linking File ' + 'build'
      print 'ln -s  /view/' + viewName + tuclPath + 'build/' + '/* ' + locaPath + '/. 2>/dev/null'
      os.system('ln -s  /view/' + viewName + tuclPath + 'build/' + '/* ' + locaPath + '/. 2>/dev/null')

      # Link tucl Sample files from vob
      print 'Linking File ' + 'sample'
      print 'ln -s  /view/' + viewName + tuclPath + 'sample/' + '/* ' + locaPath + '/. 2>/dev/null'
      os.system('ln -s  /view/' + viewName + tuclPath + 'sample/' + '/* ' + locaPath + '/. 2>/dev/null')

      # Link SCTP files from rls directory
      #print 'Linking File ' + 'sctp'
      #print 'ln -s  /view/' + viewName + sctpPath + '/sb* ' + locaPath + '/. 2>/dev/null'
      #os.system('ln -s  /view/' + viewName + sctpPath + '/sb* ' + locaPath + '/. 2>/dev/null')

      # Link Common files from Vob
      print 'Linking File ' + 'common'
      #print 'ln -s  /view/' + viewName + commonPath + '/sb* ' + locaPath + '/. 2>/dev/null'
      os.system('ln -s  /view/' + viewName + commonPath + '/inf/cm_dns/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/inf/cm_dns/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/inf/cm_tpt/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/inf/lhi/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/inf/sct/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/inf/hit/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/inf/sht/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/lib/cm_hash/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/lib/cm_inet/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/lib/cm_lib/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/lib/cm_llist/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/lib/cm_spgtt/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/lib/mem/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/lib/timer/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/tech/cm_cm/src/* ' + locaPath + '/. 2>/dev/null')
      os.system('ln -s  /view/' + viewName + commonPath + '/tech/cm_ss7/src/* ' + locaPath + '/. 2>/dev/null')


      os.system('make -f dohi.mak.00  2>/dev/null')
