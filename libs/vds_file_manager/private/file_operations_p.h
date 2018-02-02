#ifndef __VDS_FILE_MANAGER_FILE_OPERATIONS_P_H_
#define __VDS_FILE_MANAGER_FILE_OPERATIONS_P_H_

/*
Copyright (c) 2017, Vadim Malyshev, lboss75@gmail.com
All rights reserved
*/
#include "file_operations.h"
#include "transactions/file_add_transaction.h"

namespace vds {
  namespace file_manager_private {
    class _file_operations : public std::enable_shared_from_this<_file_operations> {
    public:
      async_task<> upload_file(
          const service_provider &sp,
          const guid & channel_id,
          const std::string &name,
          const std::string &mimetype,
          const filename &file_path);

	    async_task<> download_file(
					const service_provider &sp,
					const std::shared_ptr<file_manager::download_file_task> & task);


	    async_task<> download_block(
			const service_provider& sp,
			database_transaction& t,
      file_manager::download_file_task::block_info & block_id,
			const std::shared_ptr<file_manager::download_file_task> & result);

    private:
      void pack_file(
          const vds::service_provider &sp,
          const vds::filename &file_path,
          vds::database_transaction &t,
          std::list<transactions::file_add_transaction::file_block_t> &file_blocks) const;

			void restore_chunk(
					const service_provider& sp,
					database_transaction& t,
					file_manager::download_file_task::block_info & block,
					const std::shared_ptr<file_manager::download_file_task> & result);

		};
  }
}

#endif //__VDS_FILE_MANAGER_FILE_OPERATIONS_P_H_