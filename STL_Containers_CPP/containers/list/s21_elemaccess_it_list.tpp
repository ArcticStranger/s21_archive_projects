#ifndef _s21_list_EA_IT_
#define _s21_list_EA_IT_

namespace s21 {
// возвращает данные следующего узла
template <typename T>
typename list<T>::const_reference list<T>::front() {
  if (empty()) throw std::out_of_range("list is empty");
  return sentry->next->data;
}
// возвращает данные пред узла
template <typename T>
typename list<T>::const_reference list<T>::back() {
  if (empty()) throw std::out_of_range("List is empty");
  return sentry->prev->data;
}
// доступ к началу списка
template <typename T>
typename list<T>::ListIterator list<T>::begin() const {
  return ListIterator(sentry->next);
}
// доступ к концу списка
template <typename T>
typename list<T>::iterator list<T>::end() const {
  return ListIterator(sentry);
}
}  // namespace s21

#endif