#ifndef _X_GRADIENT_PAR
#define _X_GRADIENT_PAR

#include <boost/gil/extension/io/jpeg_dynamic_io.hpp>
#include <omp.h>
#include <thread>
#include"x_gradient.h"

using namespace boost::gil;

template <typename Out> struct halfdiff_cast_channels; // forward declaration

//template <typename SrcView, typename DstView>
void thread_fun(const SrcView& src, const DstView& dst, int ys, int ye){
  //typedef typename channel_type<DstView>::type dst_channel_t;

  for (int y = ys; y < ye; ++y)
  {
    //typename SrcView::x_iterator src_it = src.row_begin(y);
    //typename DstView::x_iterator dst_it = dst.row_begin(y);

    for (int x = 1; x < src.width() - 1; ++x)
    //for (int x = 1; x < 10 - 1; ++x)
    {
      printf("hi");
      //static_transform(src_it[x - 1], src_it[x + 1], dst_it[x],
      //                 halfdiff_cast_channels<dst_channel_t>());
    }
  }

}
template <typename SrcView, typename DstView>
void call_from_thread() {
      std::cout << "Launched by thread " <<std::endl;
}

template <typename SrcView, typename DstView>
void x_gradient(const SrcView& src, const DstView& dst, int num_threads) {
  int num_threads1 = num_threads;
  std::thread t[3];
  //typedef typename channel_type<DstView>::type dst_channel_t;

  //may need to move this to other function:
  typedef typename channel_type<DstView>::type dst_channel_t;

  int ys = 0;
  int ye = 0;
  //std::cout<< src.height() << std::endl;
  int heightblocks = src.height() / num_threads;
  int leftover     = src.height() % num_threads;

  for (int i = 0; i < num_threads-1; ++i)
  {
    ys = (int) heightblocks * i;
    ye = (int) heightblocks + (heightblocks * i);

    t[i] = std::thread(thread_fun<void>(src,dst) ys, ye);
    //t[i] = std::thread(thread_fun, src, dst, ys, ye);
    //t[i] = std::thread(call_from_thread, i);
    //t[i] = std::thread(call_from_thread);
    //call_from_thread();
  }
  ye += leftover;
  //t[num_threads-1] = std::thread(thread_fun, src, dst, ys, ye);
  //t[num_threads-1] = std::thread(thread_fun,src, dst, ys, ye);
  for (int i = 0; i < num_threads; ++i)
  {
    t[i].join();
  }

}

#endif // !_X_GRADIENT_PAR_
