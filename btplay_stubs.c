/*
 *  The last few compositor entry points the scene loader references and that
 *  neither the solver nor clock.c provides.
 *
 *  All three belong to the media-object machinery a *streamed* scene needs -
 *  buffering state, object-descriptor setup, and the "is a systems frame still
 *  pending" query the compositor asks before it draws. A scene loaded from a
 *  file has no stream to buffer and no object descriptors to resolve, so these
 *  are never reached on that path. They are written to be safe rather than
 *  merely quiet: the two queries answer "nothing pending", and the OD setup
 *  reports that it did nothing.
 */

#include <gpac/internal/compositor_dev.h>
#include <string.h>

#ifndef GPAC_DISABLE_COMPOSITOR

Bool gf_odm_check_buffering(GF_ObjectManager *odm, GF_FilterPid *pid)
{
	(void) odm;
	(void) pid;
	return GF_FALSE;
}

void gf_sc_sys_frame_pending(GF_Compositor *compositor, u32 cts, u32 obj_time, GF_Filter *from_filter)
{
	(void) compositor;
	(void) cts;
	(void) obj_time;
	(void) from_filter;
}

GF_Err ODS_SetupOD(GF_Scene *scene, GF_ObjectDescriptor *od)
{
	(void) scene;
	(void) od;
	return GF_OK;
}

#endif /*GPAC_DISABLE_COMPOSITOR*/

/*
 *  gf_strmemstr is a memmem over a buffer that is not NUL-terminated; the
 *  scene loader uses it to sniff which syntax a file holds. The solvers do not
 *  export it, so it is brought here verbatim from GPAC's own utils/error.c.
 */
const char *gf_strmemstr(const char *data, u32 data_size, const char *pat)
{
	u32 len_pat;

	if (!pat || !data) return NULL;
	len_pat = (u32) strlen(pat);
	if (len_pat == 0) return data;
	if (len_pat > data_size) return NULL;

	while (1) {
		char *next = memchr((void *) data, pat[0], data_size);
		u32 left;
		if (!next) return NULL;
		left = data_size - (u32) (next - data);
		if (left < len_pat) return NULL;
		if (!memcmp(next, pat, len_pat)) return next;
		data_size = left - 1;
		data = next + 1;
	}
	return NULL;
}
