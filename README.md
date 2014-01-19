dnsdyn_updater
==============

A C code snippet to update your dynamic DNS address from within an application


##Example##

<pre><code>
include "dnsdyn_updater.h"
int main(){
	dnsdyn_updater_info i;
	dnsdyn_updater_init_info(&i, "<Hostname to update>" ,"<base64 of email:password>", "<New ip addresse>");
	dnsdyn_updater_set_ip(i);
	return 0;
}
</pre></code>
