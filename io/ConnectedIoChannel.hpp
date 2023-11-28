#pragma once
// -------------------------------------------------------------------------------------
#include <stdexcept>
#include "IoChannel.hpp"
#include "IoOptions.hpp"
#include "IoRequest.hpp"
#include "RequestStack.hpp"
#include "utils/RingBufferSPSC.hpp"
// -------------------------------------------------------------------------------------
namespace mean
{
   // -------------------------------------------------------------------------------------
   class RemoteIoChannel : public IoChannel
   {
      u64 pushed = 0;
      u64 completed = 0;

   public:
      std::unique_ptr<RequestStack<IoBaseRequest>> stack;
      leanstore::utils::RingBufferSPSC<IoBaseRequest *> submit_ring;
      leanstore::utils::RingBufferSPSC<IoBaseRequest *> completion_ring;
      // -------------------------------------------------------------------------------------
      RemoteIoChannel(IoOptions options);
      // -------------------------------------------------------------------------------------
      void _push(const IoBaseRequest &req) override;
      int _submit() override;
      int _poll(int min = 0) override;
      int submitable() override;
      // -------------------------------------------------------------------------------------
      void _printSpecializedCounters(std::ostream &ss) override;
      bool readStackFull() override;
      bool writeStackFull() override;
      void registerRemoteChannel(RemoteIoChannel *rem) override
      {
         throw std::logic_error("Cannot add a RemoteIoChannel to a RemoteIoChannel");
      }
   };
   // -------------------------------------------------------------------------------------
   class RemoteIoChannelClient
   {
   public:
      static constexpr int MAX_REMOTE_IO_CHANNELS = 2;
      int remote_count = 0;
      std::array<RemoteIoChannel *, MAX_REMOTE_IO_CHANNELS> remotes;
      void registerRemote(RemoteIoChannel *remote)
      {
         if (remote_count < MAX_REMOTE_IO_CHANNELS)
         {
            remotes[remote_count] = remote;
            remote_count++;
         }
         else
         {
            throw std::logic_error("too many remote channels");
         }
      }
   };
   // -------------------------------------------------------------------------------------
} // namespace mean
// -------------------------------------------------------------------------------------
