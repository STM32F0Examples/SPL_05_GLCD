#ifndef __RETARGET_H__
	#define	__RETARGET_H__
	struct __FILE { 
		void * handle;
	};
	
	typedef void (*__WriteFunction_handle)(char);
	typedef char (*__ReadFunction_handle)(void);
	
#endif
	
	