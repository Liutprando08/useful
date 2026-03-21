#include "piece_table.h"
#include <stdlib.h>

pieceTable T;

void initPieceTable() {
  T.original_buffer = NULL;
  T.original_length = 0;
  T.add_capacity = 1024;
  T.add_buffer = malloc(T.add_capacity);
  T.capacity = 0;
  T.add_length = 0;
  T.pieces_capacity = 100;
  T.pieces = malloc(sizeof(piece) * T.pieces_capacity);
  T.pieces_count = 0;
}

void piece_table_delete_range(int start, int end) {
  if (start >= end || start < 0)
    return;

  // Calculate total document length
  int total_len = 0;
  for (int i = 0; i < T.pieces_count; i++) {
    total_len += T.pieces[i].length;
  }

  // Validate bounds
  if (end > total_len)
    end = total_len;
  if (start >= end)
    return;

  int deleted_len = end - start;

  // Find pieces that contain start and end
  int current_pos = 0;
  int start_piece = -1, end_piece = -1;
  int start_offset = 0, end_offset = 0;

  for (int i = 0; i < T.pieces_count; i++) {
    int piece_end = current_pos + T.pieces[i].length;

    if (start_piece == -1 && start < piece_end) {
      start_piece = i;
      start_offset = start - current_pos;
    }

    if (end <= piece_end) {
      end_piece = i;
      end_offset = end - current_pos;
      break;
    }

    current_pos = piece_end;
  }

  if (start_piece == -1 || end_piece == -1)
    return;

  // Case 1: Both start and end in same piece
  if (start_piece == end_piece) {
    piece p = T.pieces[start_piece];
    T.pieces[start_piece].length -= deleted_len;

    // If deleted range is not at the end, we need to split
    if (end_offset < p.length) {
      // Shift pieces to make room
      for (int i = T.pieces_count; i > start_piece + 1; i--) {
        T.pieces[i] = T.pieces[i - 1];
      }
      T.pieces_count++;

      // Create the after-delete piece
      T.pieces[start_piece + 1].buffer = p.buffer;
      T.pieces[start_piece + 1].start = p.start + end_offset;
      T.pieces[start_piece + 1].length = p.length - end_offset;
    }

    // Remove empty pieces
    if (T.pieces[start_piece].length == 0) {
      for (int i = start_piece; i < T.pieces_count - 1; i++) {
        T.pieces[i] = T.pieces[i + 1];
      }
      T.pieces_count--;
    }
  } else {
    // Case 2: Delete spans multiple pieces

    // Trim the end of the start piece
    T.pieces[start_piece].length = start_offset;

    // Remove empty start piece
    int pieces_to_remove = 1; // at least the pieces between start and end
    for (int i = start_piece + 1; i < end_piece; i++) {
      pieces_to_remove++;
    }

    // Calculate how much to trim from end piece
    int end_piece_trim = end_offset;
    if (end_offset < T.pieces[end_piece].length) {
      // Keep the part after end_offset
      T.pieces[end_piece].start += end_piece_trim;
      T.pieces[end_piece].length -= end_piece_trim;
      pieces_to_remove--;
    } else {
      // Remove entire end piece
      pieces_to_remove++;
    }

    // Shift pieces down
    for (int i = start_piece + 1; i + pieces_to_remove < T.pieces_count; i++) {
      T.pieces[i] = T.pieces[i + pieces_to_remove];
    }
    T.pieces_count -= pieces_to_remove;

    // Remove empty start piece
    if (T.pieces[start_piece].length == 0 && start_piece < T.pieces_count) {
      for (int i = start_piece; i < T.pieces_count - 1; i++) {
        T.pieces[i] = T.pieces[i + 1];
      }
      T.pieces_count--;
    }
  }
}
