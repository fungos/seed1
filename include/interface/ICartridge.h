/*! \file ICartridge.h
	\author	Danny Angelo Carminati Grein
	\brief Defines the Cartridge class interface
*/

#ifndef __ICARTRIDGE_H__
#define __ICARTRIDGE_H__


#include "IModule.h"
#include "../Enum.h"


namespace Seed {


/// Cartridge Interface
/**
Interface for cartridge/save data, it can be a real cartridge like Nintendo DS builtin EPROM
and/or even a file in a PC implementation.
*/
class ICartridge : public IModule
{
	public:
		ICartridge();
		virtual ~ICartridge();

		/// Need be called before initialization to set the type of cartridge to use.
		/**
		Pre-configure the cartridge module for the type of cartridge to be used.

		\param size eCartridgeSize type.
		\return true if cartridge is ready, false otherwise
		*/
		virtual BOOL Prepare(eCartridgeSize size);

		/// Read a cartridge data to an allocated buffer
		/**
		Read len bytes of the data begining in src to a dest buffer.

		\param src The offset from where start reading saved data.
		\param dest Destination buffer where the data will be written.
		\param len Total length of the data to be read.
		\return true if read succeeds, false otherwise
		*/
		virtual BOOL Read(u32 src, void *dest, u32 len);

		/// Write data from a buffer to a cartridge
		/**
		Read len bytes of the data begining in src to a dest buffer.

		\param src The offset from where start reading saved data.
		\param dest Destination buffer where the data will be written.
		\param len Total length of the data to be read.
		\return true if write succeeds, false otherwise
		*/
		virtual BOOL Write(u32 dest, const void *src, u32 len);

		/// Get the cartridge size
		/**
		\return Total size in bytes of the cartridge
		*/
		virtual u32	GetSize() const;


		/// Retrieves the last error ocurred
		/**
		Returns the last error ocurred if any.

		\return the last error ocurred.
		*/
		virtual eCartridgeError GetLastError() const;

		virtual u32 GetOffsetForSlot(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 slot) const;
		virtual u32 GetOffsetForSlotCRC(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 slot) const;
		virtual u32 GetRequiredSize(u32 headerSize, u32 sharedDataSize, u32 slotDataSize, u32 num_slots) const;		

	protected:
		eCartridgeError eLastError;
		u32	iSize;

	private:
		SEED_DISABLE_COPY(ICartridge);
};


} // namespace


#endif // __ICARTRIDGE_H__
