/*
 * qdf_delayed_work.h - Compatibility shim for the create/start/stop_sync/
 * destroy style delayed-work API expected by the frame-injection patch.
 *
 * This tree's native qdf_defer.h provides a lighter-weight delayed-work
 * primitive (qdf_delayed_work_t + qdf_create_delayed_work/
 * qdf_sched_delayed_work/qdf_cancel_delayed_work/qdf_flush_delayed_work)
 * rather than the struct qdf_delayed_work object with a create/start/
 * stop_sync/destroy lifecycle. This shim adapts one to the other.
 */
#ifndef _QDF_DELAYED_WORK_H
#define _QDF_DELAYED_WORK_H
#include <qdf_defer.h>
#include <qdf_status.h>
#include <qdf_types.h>

struct qdf_delayed_work {
        qdf_delayed_work_t dwork;
};

static inline QDF_STATUS
qdf_delayed_work_create(struct qdf_delayed_work *dw,
                        qdf_defer_fn_t func, void *arg)
{
        if (!dw || !func)
                return QDF_STATUS_E_INVAL;

        qdf_create_delayed_work(&dw->dwork, func, arg);

        return QDF_STATUS_SUCCESS;
}

static inline void
qdf_delayed_work_start(struct qdf_delayed_work *dw, uint32_t delay_ms)
{
        if (!dw)
                return;

        qdf_sched_delayed_work(&dw->dwork, delay_ms);
}

static inline void
qdf_delayed_work_stop_sync(struct qdf_delayed_work *dw)
{
        if (!dw)
                return;

        qdf_cancel_delayed_work(&dw->dwork);
        qdf_flush_delayed_work(&dw->dwork);
}

static inline void
qdf_delayed_work_destroy(struct qdf_delayed_work *dw)
{
        if (!dw)
                return;

        qdf_delayed_work_stop_sync(dw);
}

#endif /* _QDF_DELAYED_WORK_H */
