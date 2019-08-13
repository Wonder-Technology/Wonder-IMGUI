(function (global, factory) {
	typeof exports === 'object' && typeof module !== 'undefined' ? factory(exports) :
	typeof define === 'function' && define.amd ? define(['exports'], factory) :
	(factory((global.wdimgui = {})));
}(this, (function (exports) { 'use strict';

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function Stream (source) {
	  this.source = source;
	}

	Stream.prototype.run = function (sink, scheduler) {
	  return this.source.run(sink, scheduler)
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */

	// Non-mutating array operations

	// cons :: a -> [a] -> [a]
	// a with x prepended
	function cons(x, a) {
	  var l = a.length;
	  var b = new Array(l + 1);
	  b[0] = x;
	  for (var i = 0; i < l; ++i) {
	    b[i + 1] = a[i];
	  }
	  return b;
	}

	// append :: a -> [a] -> [a]
	// a with x appended
	function append(x, a) {
	  var l = a.length;
	  var b = new Array(l + 1);
	  for (var i = 0; i < l; ++i) {
	    b[i] = a[i];
	  }

	  b[l] = x;
	  return b;
	}

	// drop :: Int -> [a] -> [a]
	// drop first n elements
	function drop(n, a) {
	  // eslint-disable-line complexity
	  if (n < 0) {
	    throw new TypeError('n must be >= 0');
	  }

	  var l = a.length;
	  if (n === 0 || l === 0) {
	    return a;
	  }

	  if (n >= l) {
	    return [];
	  }

	  return unsafeDrop(n, a, l - n);
	}

	// unsafeDrop :: Int -> [a] -> Int -> [a]
	// Internal helper for drop
	function unsafeDrop(n, a, l) {
	  var b = new Array(l);
	  for (var i = 0; i < l; ++i) {
	    b[i] = a[n + i];
	  }
	  return b;
	}

	// tail :: [a] -> [a]
	// drop head element
	function tail(a) {
	  return drop(1, a);
	}

	// copy :: [a] -> [a]
	// duplicate a (shallow duplication)
	function copy(a) {
	  var l = a.length;
	  var b = new Array(l);
	  for (var i = 0; i < l; ++i) {
	    b[i] = a[i];
	  }
	  return b;
	}

	// map :: (a -> b) -> [a] -> [b]
	// transform each element with f
	function map(f, a) {
	  var l = a.length;
	  var b = new Array(l);
	  for (var i = 0; i < l; ++i) {
	    b[i] = f(a[i]);
	  }
	  return b;
	}

	// reduce :: (a -> b -> a) -> a -> [b] -> a
	// accumulate via left-fold
	function reduce(f, z, a) {
	  var r = z;
	  for (var i = 0, l = a.length; i < l; ++i) {
	    r = f(r, a[i], i);
	  }
	  return r;
	}

	// replace :: a -> Int -> [a]
	// replace element at index
	function replace(x, i, a) {
	  // eslint-disable-line complexity
	  if (i < 0) {
	    throw new TypeError('i must be >= 0');
	  }

	  var l = a.length;
	  var b = new Array(l);
	  for (var j = 0; j < l; ++j) {
	    b[j] = i === j ? x : a[j];
	  }
	  return b;
	}

	// remove :: Int -> [a] -> [a]
	// remove element at index
	function remove(i, a) {
	  // eslint-disable-line complexity
	  if (i < 0) {
	    throw new TypeError('i must be >= 0');
	  }

	  var l = a.length;
	  if (l === 0 || i >= l) {
	    // exit early if index beyond end of array
	    return a;
	  }

	  if (l === 1) {
	    // exit early if index in bounds and length === 1
	    return [];
	  }

	  return unsafeRemove(i, a, l - 1);
	}

	// unsafeRemove :: Int -> [a] -> Int -> [a]
	// Internal helper to remove element at index
	function unsafeRemove(i, a, l) {
	  var b = new Array(l);
	  var j = void 0;
	  for (j = 0; j < i; ++j) {
	    b[j] = a[j];
	  }
	  for (j = i; j < l; ++j) {
	    b[j] = a[j + 1];
	  }

	  return b;
	}

	// removeAll :: (a -> boolean) -> [a] -> [a]
	// remove all elements matching a predicate
	// @deprecated
	function removeAll(f, a) {
	  var l = a.length;
	  var b = new Array(l);
	  var j = 0;
	  for (var x, i = 0; i < l; ++i) {
	    x = a[i];
	    if (!f(x)) {
	      b[j] = x;
	      ++j;
	    }
	  }

	  b.length = j;
	  return b;
	}

	// findIndex :: a -> [a] -> Int
	// find index of x in a, from the left
	function findIndex(x, a) {
	  for (var i = 0, l = a.length; i < l; ++i) {
	    if (x === a[i]) {
	      return i;
	    }
	  }
	  return -1;
	}

	// isArrayLike :: * -> boolean
	// Return true iff x is array-like
	function isArrayLike(x) {
	  return x != null && typeof x.length === 'number' && typeof x !== 'function';
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */

	// id :: a -> a
	var id = function id(x) {
	  return x;
	};

	// compose :: (b -> c) -> (a -> b) -> (a -> c)
	var compose = function compose(f, g) {
	  return function (x) {
	    return f(g(x));
	  };
	};

	// apply :: (a -> b) -> a -> b
	var apply = function apply(f, x) {
	  return f(x);
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Create a new Disposable which will dispose its underlying resource.
	 * @param {function} dispose function
	 * @param {*?} data any data to be passed to disposer function
	 * @constructor
	 */
	function Disposable (dispose, data) {
	  this._dispose = dispose;
	  this._data = data;
	}

	Disposable.prototype.dispose = function () {
	  return this._dispose(this._data)
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function SettableDisposable () {
	  this.disposable = void 0;
	  this.disposed = false;
	  this._resolve = void 0;

	  var self = this;
	  this.result = new Promise(function (resolve) {
	    self._resolve = resolve;
	  });
	}

	SettableDisposable.prototype.setDisposable = function (disposable) {
	  if (this.disposable !== void 0) {
	    throw new Error('setDisposable called more than once')
	  }

	  this.disposable = disposable;

	  if (this.disposed) {
	    this._resolve(disposable.dispose());
	  }
	};

	SettableDisposable.prototype.dispose = function () {
	  if (this.disposed) {
	    return this.result
	  }

	  this.disposed = true;

	  if (this.disposable !== void 0) {
	    this.result = this.disposable.dispose();
	  }

	  return this.result
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function isPromise (p) {
	  return p !== null && typeof p === 'object' && typeof p.then === 'function'
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */
	var map$1 = map;
	var identity = id;

	/**
	 * Call disposable.dispose.  If it returns a promise, catch promise
	 * error and forward it through the provided sink.
	 * @param {number} t time
	 * @param {{dispose: function}} disposable
	 * @param {{error: function}} sink
	 * @return {*} result of disposable.dispose
	 */
	function tryDispose (t, disposable, sink) {
	  var result = disposeSafely(disposable);
	  return isPromise(result)
	    ? result.catch(function (e) {
	      sink.error(t, e);
	    })
	    : result
	}

	/**
	 * Create a new Disposable which will dispose its underlying resource
	 * at most once.
	 * @param {function} dispose function
	 * @param {*?} data any data to be passed to disposer function
	 * @return {Disposable}
	 */
	function create (dispose, data) {
	  return once(new Disposable(dispose, data))
	}

	/**
	 * Create a noop disposable. Can be used to satisfy a Disposable
	 * requirement when no actual resource needs to be disposed.
	 * @return {Disposable|exports|module.exports}
	 */
	function empty$1 () {
	  return new Disposable(identity, void 0)
	}

	/**
	 * Create a disposable that will dispose all input disposables in parallel.
	 * @param {Array<Disposable>} disposables
	 * @return {Disposable}
	 */
	function all (disposables) {
	  return create(disposeAll, disposables)
	}

	function disposeAll (disposables) {
	  return Promise.all(map$1(disposeSafely, disposables))
	}

	function disposeSafely (disposable) {
	  try {
	    return disposable.dispose()
	  } catch (e) {
	    return Promise.reject(e)
	  }
	}

	/**
	 * Create a disposable from a promise for another disposable
	 * @param {Promise<Disposable>} disposablePromise
	 * @return {Disposable}
	 */


	/**
	 * Create a disposable proxy that allows its underlying disposable to
	 * be set later.
	 * @return {SettableDisposable}
	 */
	function settable () {
	  return new SettableDisposable()
	}

	/**
	 * Wrap an existing disposable (which may not already have been once()d)
	 * so that it will only dispose its underlying resource at most once.
	 * @param {{ dispose: function() }} disposable
	 * @return {Disposable} wrapped disposable
	 */
	function once (disposable) {
	  return new Disposable(disposeMemoized, memoized(disposable))
	}

	function disposeMemoized (memoized) {
	  if (!memoized.disposed) {
	    memoized.disposed = true;
	    memoized.value = disposeSafely(memoized.disposable);
	    memoized.disposable = void 0;
	  }

	  return memoized.value
	}

	function memoized (disposable) {
	  return { disposed: false, disposable: disposable, value: void 0 }
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function fatalError (e) {
	  setTimeout(function () {
	    throw e
	  }, 0);
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function PropagateTask (run, value, sink) {
	  this._run = run;
	  this.value = value;
	  this.sink = sink;
	  this.active = true;
	}

	PropagateTask.event = function (value, sink) {
	  return new PropagateTask(emit, value, sink)
	};

	PropagateTask.end = function (value, sink) {
	  return new PropagateTask(end, value, sink)
	};

	PropagateTask.error = function (value, sink) {
	  return new PropagateTask(error, value, sink)
	};

	PropagateTask.prototype.dispose = function () {
	  this.active = false;
	};

	PropagateTask.prototype.run = function (t) {
	  if (!this.active) {
	    return
	  }
	  this._run(t, this.value, this.sink);
	};

	PropagateTask.prototype.error = function (t, e) {
	  if (!this.active) {
	    return fatalError(e)
	  }
	  this.sink.error(t, e);
	};

	function error (t, e, sink) {
	  sink.error(t, e);
	}

	function emit (t, x, sink) {
	  sink.event(t, x);
	}

	function end (t, x, sink) {
	  sink.end(t, x);
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Stream containing only x
	 * @param {*} x
	 * @returns {Stream}
	 */
	function of (x) {
	  return new Stream(new Just(x))
	}

	function Just (x) {
	  this.value = x;
	}

	Just.prototype.run = function (sink, scheduler) {
	  return scheduler.asap(new PropagateTask(runJust, this.value, sink))
	};

	function runJust (t, x, sink) {
	  sink.event(t, x);
	  sink.end(t, void 0);
	}

	/**
	 * Stream containing no events and ends immediately
	 * @returns {Stream}
	 */
	function empty () {
	  return EMPTY
	}

	function EmptySource () {}

	EmptySource.prototype.run = function (sink, scheduler) {
	  var task = PropagateTask.end(void 0, sink);
	  scheduler.asap(task);

	  return create(disposeEmpty, task)
	};

	function disposeEmpty (task) {
	  return task.dispose()
	}

	var EMPTY = new Stream(new EmptySource());

	/**
	 * Stream containing no events and never ends
	 * @returns {Stream}
	 */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function fromArray (a) {
	  return new Stream(new ArraySource(a))
	}

	function ArraySource (a) {
	  this.array = a;
	}

	ArraySource.prototype.run = function (sink, scheduler) {
	  return scheduler.asap(new PropagateTask(runProducer, this.array, sink))
	};

	function runProducer (t, array, sink) {
	  for (var i = 0, l = array.length; i < l && this.active; ++i) {
	    sink.event(t, array[i]);
	  }

	  this.active && sink.end(t);
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/* global Set, Symbol */
	var iteratorSymbol;
	// Firefox ships a partial implementation using the name @@iterator.
	// https://bugzilla.mozilla.org/show_bug.cgi?id=907077#c14
	if (typeof Set === 'function' && typeof new Set()['@@iterator'] === 'function') {
	  iteratorSymbol = '@@iterator';
	} else {
	  iteratorSymbol = typeof Symbol === 'function' ? Symbol.iterator
	  : '_es6shim_iterator_';
	}

	function isIterable (o) {
	  return typeof o[iteratorSymbol] === 'function'
	}

	function getIterator (o) {
	  return o[iteratorSymbol]()
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function fromIterable (iterable) {
	  return new Stream(new IterableSource(iterable))
	}

	function IterableSource (iterable) {
	  this.iterable = iterable;
	}

	IterableSource.prototype.run = function (sink, scheduler) {
	  return scheduler.asap(new PropagateTask(runProducer$1, getIterator(this.iterable), sink))
	};

	function runProducer$1 (t, iterator, sink) {
	  var r = iterator.next();

	  while (!r.done && this.active) {
	    sink.event(t, r.value);
	    r = iterator.next();
	  }

	  sink.end(t, r.value);
	}

	function symbolObservablePonyfill(root) {
		var result;
		var Symbol = root.Symbol;

		if (typeof Symbol === 'function') {
			if (Symbol.observable) {
				result = Symbol.observable;
			} else {
				result = Symbol('observable');
				Symbol.observable = result;
			}
		} else {
			result = '@@observable';
		}

		return result;
	}

	/* global window */
	var root;

	if (typeof self !== 'undefined') {
	  root = self;
	} else if (typeof window !== 'undefined') {
	  root = window;
	} else if (typeof global !== 'undefined') {
	  root = global;
	} else if (typeof module !== 'undefined') {
	  root = module;
	} else {
	  root = Function('return this')();
	}

	var result = symbolObservablePonyfill(root);

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function getObservable (o) { // eslint-disable-line complexity
	  var obs = null;
	  if (o) {
	  // Access foreign method only once
	    var method = o[result];
	    if (typeof method === 'function') {
	      obs = method.call(o);
	      if (!(obs && typeof obs.subscribe === 'function')) {
	        throw new TypeError('invalid observable ' + obs)
	      }
	    }
	  }

	  return obs
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function tryEvent (t, x, sink) {
	  try {
	    sink.event(t, x);
	  } catch (e) {
	    sink.error(t, e);
	  }
	}

	function tryEnd (t, x, sink) {
	  try {
	    sink.end(t, x);
	  } catch (e) {
	    sink.error(t, e);
	  }
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function fromObservable (observable) {
	  return new Stream(new ObservableSource(observable))
	}

	function ObservableSource (observable) {
	  this.observable = observable;
	}

	ObservableSource.prototype.run = function (sink, scheduler) {
	  var sub = this.observable.subscribe(new SubscriberSink(sink, scheduler));
	  if (typeof sub === 'function') {
	    return create(sub)
	  } else if (sub && typeof sub.unsubscribe === 'function') {
	    return create(unsubscribe, sub)
	  }

	  throw new TypeError('Observable returned invalid subscription ' + String(sub))
	};

	function SubscriberSink (sink, scheduler) {
	  this.sink = sink;
	  this.scheduler = scheduler;
	}

	SubscriberSink.prototype.next = function (x) {
	  tryEvent(this.scheduler.now(), x, this.sink);
	};

	SubscriberSink.prototype.complete = function (x) {
	  tryEnd(this.scheduler.now(), x, this.sink);
	};

	SubscriberSink.prototype.error = function (e) {
	  this.sink.error(this.scheduler.now(), e);
	};

	function unsubscribe (subscription) {
	  return subscription.unsubscribe()
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function from (a) { // eslint-disable-line complexity
	  if (a instanceof Stream) {
	    return a
	  }

	  var observable = getObservable(a);
	  if (observable != null) {
	    return fromObservable(observable)
	  }

	  if (Array.isArray(a) || isArrayLike(a)) {
	    return fromArray(a)
	  }

	  if (isIterable(a)) {
	    return fromIterable(a)
	  }

	  throw new TypeError('from(x) must be observable, iterable, or array-like: ' + a)
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Create a stream that emits the current time periodically
	 * @param {Number} period periodicity of events in millis
	 * @param {*} deprecatedValue @deprecated value to emit each period
	 * @returns {Stream} new stream that emits the current time every period
	 */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function ScheduledTask (delay, period, task, scheduler) {
	  this.time = delay;
	  this.period = period;
	  this.task = task;
	  this.scheduler = scheduler;
	  this.active = true;
	}

	ScheduledTask.prototype.run = function () {
	  return this.task.run(this.time)
	};

	ScheduledTask.prototype.error = function (e) {
	  return this.task.error(this.time, e)
	};

	ScheduledTask.prototype.dispose = function () {
	  this.scheduler.cancel(this);
	  return this.task.dispose()
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function defer (task) {
	  return Promise.resolve(task).then(runTask)
	}

	function runTask (task) {
	  try {
	    return task.run()
	  } catch (e) {
	    return task.error(e)
	  }
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function Scheduler (timer, timeline) {
	  this.timer = timer;
	  this.timeline = timeline;

	  this._timer = null;
	  this._nextArrival = Infinity;

	  var self = this;
	  this._runReadyTasksBound = function () {
	    self._runReadyTasks(self.now());
	  };
	}

	Scheduler.prototype.now = function () {
	  return this.timer.now()
	};

	Scheduler.prototype.asap = function (task) {
	  return this.schedule(0, -1, task)
	};

	Scheduler.prototype.delay = function (delay, task) {
	  return this.schedule(delay, -1, task)
	};

	Scheduler.prototype.periodic = function (period, task) {
	  return this.schedule(0, period, task)
	};

	Scheduler.prototype.schedule = function (delay, period, task) {
	  var now = this.now();
	  var st = new ScheduledTask(now + Math.max(0, delay), period, task, this);

	  this.timeline.add(st);
	  this._scheduleNextRun(now);
	  return st
	};

	Scheduler.prototype.cancel = function (task) {
	  task.active = false;
	  if (this.timeline.remove(task)) {
	    this._reschedule();
	  }
	};

	Scheduler.prototype.cancelAll = function (f) {
	  this.timeline.removeAll(f);
	  this._reschedule();
	};

	Scheduler.prototype._reschedule = function () {
	  if (this.timeline.isEmpty()) {
	    this._unschedule();
	  } else {
	    this._scheduleNextRun(this.now());
	  }
	};

	Scheduler.prototype._unschedule = function () {
	  this.timer.clearTimer(this._timer);
	  this._timer = null;
	};

	Scheduler.prototype._scheduleNextRun = function (now) { // eslint-disable-line complexity
	  if (this.timeline.isEmpty()) {
	    return
	  }

	  var nextArrival = this.timeline.nextArrival();

	  if (this._timer === null) {
	    this._scheduleNextArrival(nextArrival, now);
	  } else if (nextArrival < this._nextArrival) {
	    this._unschedule();
	    this._scheduleNextArrival(nextArrival, now);
	  }
	};

	Scheduler.prototype._scheduleNextArrival = function (nextArrival, now) {
	  this._nextArrival = nextArrival;
	  var delay = Math.max(0, nextArrival - now);
	  this._timer = this.timer.setTimer(this._runReadyTasksBound, delay);
	};

	Scheduler.prototype._runReadyTasks = function (now) {
	  this._timer = null;
	  this.timeline.runTasks(now, runTask);
	  this._scheduleNextRun(this.now());
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/* global setTimeout, clearTimeout */

	function ClockTimer () {}

	ClockTimer.prototype.now = Date.now;

	ClockTimer.prototype.setTimer = function (f, dt) {
	  return dt <= 0 ? runAsap(f) : setTimeout(f, dt)
	};

	ClockTimer.prototype.clearTimer = function (t) {
	  return t instanceof Asap ? t.cancel() : clearTimeout(t)
	};

	function Asap (f) {
	  this.f = f;
	  this.active = true;
	}

	Asap.prototype.run = function () {
	  return this.active && this.f()
	};

	Asap.prototype.error = function (e) {
	  throw e
	};

	Asap.prototype.cancel = function () {
	  this.active = false;
	};

	function runAsap (f) {
	  var task = new Asap(f);
	  defer(task);
	  return task
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function Timeline () {
	  this.tasks = [];
	}

	Timeline.prototype.nextArrival = function () {
	  return this.isEmpty() ? Infinity : this.tasks[0].time
	};

	Timeline.prototype.isEmpty = function () {
	  return this.tasks.length === 0
	};

	Timeline.prototype.add = function (st) {
	  insertByTime(st, this.tasks);
	};

	Timeline.prototype.remove = function (st) {
	  var i = binarySearch(st.time, this.tasks);

	  if (i >= 0 && i < this.tasks.length) {
	    var at = findIndex(st, this.tasks[i].events);
	    if (at >= 0) {
	      this.tasks[i].events.splice(at, 1);
	      return true
	    }
	  }

	  return false
	};

	Timeline.prototype.removeAll = function (f) {
	  for (var i = 0, l = this.tasks.length; i < l; ++i) {
	    removeAllFrom(f, this.tasks[i]);
	  }
	};

	Timeline.prototype.runTasks = function (t, runTask) {
	  var tasks = this.tasks;
	  var l = tasks.length;
	  var i = 0;

	  while (i < l && tasks[i].time <= t) {
	    ++i;
	  }

	  this.tasks = tasks.slice(i);

	  // Run all ready tasks
	  for (var j = 0; j < i; ++j) {
	    this.tasks = runTasks(runTask, tasks[j], this.tasks);
	  }
	};

	function runTasks (runTask, timeslot, tasks) { // eslint-disable-line complexity
	  var events = timeslot.events;
	  for (var i = 0; i < events.length; ++i) {
	    var task = events[i];

	    if (task.active) {
	      runTask(task);

	      // Reschedule periodic repeating tasks
	      // Check active again, since a task may have canceled itself
	      if (task.period >= 0 && task.active) {
	        task.time = task.time + task.period;
	        insertByTime(task, tasks);
	      }
	    }
	  }

	  return tasks
	}

	function insertByTime (task, timeslots) { // eslint-disable-line complexity
	  var l = timeslots.length;

	  if (l === 0) {
	    timeslots.push(newTimeslot(task.time, [task]));
	    return
	  }

	  var i = binarySearch(task.time, timeslots);

	  if (i >= l) {
	    timeslots.push(newTimeslot(task.time, [task]));
	  } else if (task.time === timeslots[i].time) {
	    timeslots[i].events.push(task);
	  } else {
	    timeslots.splice(i, 0, newTimeslot(task.time, [task]));
	  }
	}

	function removeAllFrom (f, timeslot) {
	  timeslot.events = removeAll(f, timeslot.events);
	}

	function binarySearch (t, sortedArray) { // eslint-disable-line complexity
	  var lo = 0;
	  var hi = sortedArray.length;
	  var mid, y;

	  while (lo < hi) {
	    mid = Math.floor((lo + hi) / 2);
	    y = sortedArray[mid];

	    if (t === y.time) {
	      return mid
	    } else if (t < y.time) {
	      hi = mid;
	    } else {
	      lo = mid + 1;
	    }
	  }
	  return hi
	}

	function newTimeslot (t, events) {
	  return { time: t, events: events }
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	var defaultScheduler = new Scheduler(new ClockTimer(), new Timeline());

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function subscribe (subscriber, stream) {
	  if (Object(subscriber) !== subscriber) {
	    throw new TypeError('subscriber must be an object')
	  }

	  var disposable = settable();
	  var observer = new SubscribeObserver(fatalError, subscriber, disposable);

	  disposable.setDisposable(stream.source.run(observer, defaultScheduler));

	  return new Subscription(disposable)
	}

	function SubscribeObserver (fatalError$$1, subscriber, disposable) {
	  this.fatalError = fatalError$$1;
	  this.subscriber = subscriber;
	  this.disposable = disposable;
	}

	SubscribeObserver.prototype.event = function (t, x) {
	  if (!this.disposable.disposed && typeof this.subscriber.next === 'function') {
	    this.subscriber.next(x);
	  }
	};

	SubscribeObserver.prototype.end = function (t, x) {
	  if (!this.disposable.disposed) {
	    var s = this.subscriber;
	    var fatalError$$1 = this.fatalError;
	    Promise.resolve(this.disposable.dispose()).then(function () {
	      if (typeof s.complete === 'function') {
	        s.complete(x);
	      }
	    }).catch(function (e) {
	      throwError(e, s, fatalError$$1);
	    });
	  }
	};

	SubscribeObserver.prototype.error = function (t, e) {
	  var s = this.subscriber;
	  var fatalError$$1 = this.fatalError;
	  Promise.resolve(this.disposable.dispose()).then(function () {
	    throwError(e, s, fatalError$$1);
	  });
	};

	function Subscription (disposable) {
	  this.disposable = disposable;
	}

	Subscription.prototype.unsubscribe = function () {
	  this.disposable.dispose();
	};

	function throwError (e1, subscriber, throwError) {
	  if (typeof subscriber.error === 'function') {
	    try {
	      subscriber.error(e1);
	    } catch (e2) {
	      throwError(e2);
	    }
	  } else {
	    throwError(e1);
	  }
	}

	/** @license MIT License (c) copyright 2010-2017 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function thru (f, stream) {
	  return f(stream)
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Create a stream from an EventTarget, such as a DOM Node, or EventEmitter.
	 * @param {String} event event type name, e.g. 'click'
	 * @param {EventTarget|EventEmitter} source EventTarget or EventEmitter
	 * @param {*?} capture for DOM events, whether to use
	 *  capturing--passed as 3rd parameter to addEventListener.
	 * @returns {Stream} stream containing all events of the specified type
	 * from the source.
	 */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function withDefaultScheduler (source) {
	  return withScheduler(source, defaultScheduler)
	}

	function withScheduler (source, scheduler) {
	  return new Promise(function (resolve, reject) {
	    runSource(source, scheduler, resolve, reject);
	  })
	}

	function runSource (source, scheduler, resolve, reject) {
	  var disposable = settable();
	  var observer = new Drain(resolve, reject, disposable);

	  disposable.setDisposable(source.run(observer, scheduler));
	}

	function Drain (end, error, disposable) {
	  this._end = end;
	  this._error = error;
	  this._disposable = disposable;
	  this.active = true;
	}

	Drain.prototype.event = function (t, x) {};

	Drain.prototype.end = function (t, x) {
	  if (!this.active) {
	    return
	  }
	  this.active = false;
	  disposeThen(this._end, this._error, this._disposable, x);
	};

	Drain.prototype.error = function (t, e) {
	  this.active = false;
	  disposeThen(this._error, this._error, this._disposable, e);
	};

	function disposeThen (end, error, disposable, x) {
	  Promise.resolve(disposable.dispose()).then(function () {
	    end(x);
	  }, error);
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * A sink mixin that simply forwards event, end, and error to
	 * another sink.
	 * @param sink
	 * @constructor
	 */
	function Pipe (sink) {
	  this.sink = sink;
	}

	Pipe.prototype.event = function (t, x) {
	  return this.sink.event(t, x)
	};

	Pipe.prototype.end = function (t, x) {
	  return this.sink.end(t, x)
	};

	Pipe.prototype.error = function (t, e) {
	  return this.sink.error(t, e)
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function Filter (p, source) {
	  this.p = p;
	  this.source = source;
	}

	/**
	 * Create a filtered source, fusing adjacent filter.filter if possible
	 * @param {function(x:*):boolean} p filtering predicate
	 * @param {{run:function}} source source to filter
	 * @returns {Filter} filtered source
	 */
	Filter.create = function createFilter (p, source) {
	  if (source instanceof Filter) {
	    return new Filter(and(source.p, p), source.source)
	  }

	  return new Filter(p, source)
	};

	Filter.prototype.run = function (sink, scheduler) {
	  return this.source.run(new FilterSink(this.p, sink), scheduler)
	};

	function FilterSink (p, sink) {
	  this.p = p;
	  this.sink = sink;
	}

	FilterSink.prototype.end = Pipe.prototype.end;
	FilterSink.prototype.error = Pipe.prototype.error;

	FilterSink.prototype.event = function (t, x) {
	  var p = this.p;
	  p(x) && this.sink.event(t, x);
	};

	function and (p, q) {
	  return function (x) {
	    return p(x) && q(x)
	  }
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function FilterMap (p, f, source) {
	  this.p = p;
	  this.f = f;
	  this.source = source;
	}

	FilterMap.prototype.run = function (sink, scheduler) {
	  return this.source.run(new FilterMapSink(this.p, this.f, sink), scheduler)
	};

	function FilterMapSink (p, f, sink) {
	  this.p = p;
	  this.f = f;
	  this.sink = sink;
	}

	FilterMapSink.prototype.event = function (t, x) {
	  var f = this.f;
	  var p = this.p;
	  p(x) && this.sink.event(t, f(x));
	};

	FilterMapSink.prototype.end = Pipe.prototype.end;
	FilterMapSink.prototype.error = Pipe.prototype.error;

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function Map (f, source) {
	  this.f = f;
	  this.source = source;
	}

	/**
	 * Create a mapped source, fusing adjacent map.map, filter.map,
	 * and filter.map.map if possible
	 * @param {function(*):*} f mapping function
	 * @param {{run:function}} source source to map
	 * @returns {Map|FilterMap} mapped source, possibly fused
	 */
	Map.create = function createMap (f, source) {
	  if (source instanceof Map) {
	    return new Map(compose(f, source.f), source.source)
	  }

	  if (source instanceof Filter) {
	    return new FilterMap(source.p, f, source.source)
	  }

	  return new Map(f, source)
	};

	Map.prototype.run = function (sink, scheduler) { // eslint-disable-line no-extend-native
	  return this.source.run(new MapSink(this.f, sink), scheduler)
	};

	function MapSink (f, sink) {
	  this.f = f;
	  this.sink = sink;
	}

	MapSink.prototype.end = Pipe.prototype.end;
	MapSink.prototype.error = Pipe.prototype.error;

	MapSink.prototype.event = function (t, x) {
	  var f = this.f;
	  this.sink.event(t, f(x));
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Transform each value in the stream by applying f to each
	 * @param {function(*):*} f mapping function
	 * @param {Stream} stream stream to map
	 * @returns {Stream} stream containing items transformed by f
	 */
	function map$2 (f, stream) {
	  return new Stream(Map.create(f, stream.source))
	}

	/**
	* Replace each value in the stream with x
	* @param {*} x
	* @param {Stream} stream
	* @returns {Stream} stream containing items replaced with x
	*/
	function constant (x, stream) {
	  return map$2(function () {
	    return x
	  }, stream)
	}

	/**
	* Perform a side effect for each item in the stream
	* @param {function(x:*):*} f side effect to execute for each item. The
	*  return value will be discarded.
	* @param {Stream} stream stream to tap
	* @returns {Stream} new stream containing the same items as this stream
	*/
	function tap (f, stream) {
	  return new Stream(new Tap(f, stream.source))
	}

	function Tap (f, source) {
	  this.source = source;
	  this.f = f;
	}

	Tap.prototype.run = function (sink, scheduler) {
	  return this.source.run(new TapSink(this.f, sink), scheduler)
	};

	function TapSink (f, sink) {
	  this.sink = sink;
	  this.f = f;
	}

	TapSink.prototype.end = Pipe.prototype.end;
	TapSink.prototype.error = Pipe.prototype.error;

	TapSink.prototype.event = function (t, x) {
	  var f = this.f;
	  f(x);
	  this.sink.event(t, x);
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Observe all the event values in the stream in time order. The
	 * provided function `f` will be called for each event value
	 * @param {function(x:T):*} f function to call with each event value
	 * @param {Stream<T>} stream stream to observe
	 * @return {Promise} promise that fulfills after the stream ends without
	 *  an error, or rejects if the stream ends with an error.
	 */
	function observe (f, stream) {
	  return drain(tap(f, stream))
	}

	/**
	 * "Run" a stream by creating demand and consuming all events
	 * @param {Stream<T>} stream stream to drain
	 * @return {Promise} promise that fulfills after the stream ends without
	 *  an error, or rejects if the stream ends with an error.
	 */
	function drain (stream) {
	  return withDefaultScheduler(stream.source)
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Generalized feedback loop. Call a stepper function for each event. The stepper
	 * will be called with 2 params: the current seed and the an event value.  It must
	 * return a new { seed, value } pair. The `seed` will be fed back into the next
	 * invocation of stepper, and the `value` will be propagated as the event value.
	 * @param {function(seed:*, value:*):{seed:*, value:*}} stepper loop step function
	 * @param {*} seed initial seed value passed to first stepper call
	 * @param {Stream} stream event stream
	 * @returns {Stream} new stream whose values are the `value` field of the objects
	 * returned by the stepper
	 */
	function loop (stepper, seed, stream) {
	  return new Stream(new Loop(stepper, seed, stream.source))
	}

	function Loop (stepper, seed, source) {
	  this.step = stepper;
	  this.seed = seed;
	  this.source = source;
	}

	Loop.prototype.run = function (sink, scheduler) {
	  return this.source.run(new LoopSink(this.step, this.seed, sink), scheduler)
	};

	function LoopSink (stepper, seed, sink) {
	  this.step = stepper;
	  this.seed = seed;
	  this.sink = sink;
	}

	LoopSink.prototype.error = Pipe.prototype.error;

	LoopSink.prototype.event = function (t, x) {
	  var result = this.step(this.seed, x);
	  this.seed = result.seed;
	  this.sink.event(t, result.value);
	};

	LoopSink.prototype.end = function (t) {
	  this.sink.end(t, this.seed);
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Create a stream containing successive reduce results of applying f to
	 * the previous reduce result and the current stream item.
	 * @param {function(result:*, x:*):*} f reducer function
	 * @param {*} initial initial value
	 * @param {Stream} stream stream to scan
	 * @returns {Stream} new stream containing successive reduce results
	 */
	function scan (f, initial, stream) {
	  return new Stream(new Scan(f, initial, stream.source))
	}

	function Scan (f, z, source) {
	  this.source = source;
	  this.f = f;
	  this.value = z;
	}

	Scan.prototype.run = function (sink, scheduler) {
	  var d1 = scheduler.asap(PropagateTask.event(this.value, sink));
	  var d2 = this.source.run(new ScanSink(this.f, this.value, sink), scheduler);
	  return all([d1, d2])
	};

	function ScanSink (f, z, sink) {
	  this.f = f;
	  this.value = z;
	  this.sink = sink;
	}

	ScanSink.prototype.event = function (t, x) {
	  var f = this.f;
	  this.value = f(this.value, x);
	  this.sink.event(t, this.value);
	};

	ScanSink.prototype.error = Pipe.prototype.error;
	ScanSink.prototype.end = Pipe.prototype.end;

	/**
	* Reduce a stream to produce a single result.  Note that reducing an infinite
	* stream will return a Promise that never fulfills, but that may reject if an error
	* occurs.
	* @param {function(result:*, x:*):*} f reducer function
	* @param {*} initial initial value
	* @param {Stream} stream to reduce
	* @returns {Promise} promise for the file result of the reduce
	*/
	function reduce$1 (f, initial, stream) {
	  return withDefaultScheduler(new Reduce(f, initial, stream.source))
	}

	function Reduce (f, z, source) {
	  this.source = source;
	  this.f = f;
	  this.value = z;
	}

	Reduce.prototype.run = function (sink, scheduler) {
	  return this.source.run(new ReduceSink(this.f, this.value, sink), scheduler)
	};

	function ReduceSink (f, z, sink) {
	  this.f = f;
	  this.value = z;
	  this.sink = sink;
	}

	ReduceSink.prototype.event = function (t, x) {
	  var f = this.f;
	  this.value = f(this.value, x);
	  this.sink.event(t, this.value);
	};

	ReduceSink.prototype.error = Pipe.prototype.error;

	ReduceSink.prototype.end = function (t) {
	  this.sink.end(t, this.value);
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Compute a stream by unfolding tuples of future values from a seed value
	 * Event times may be controlled by returning a Promise from f
	 * @param {function(seed:*):{value:*, seed:*, done:boolean}|Promise<{value:*, seed:*, done:boolean}>} f unfolding function accepts
	 *  a seed and returns a new tuple with a value, new seed, and boolean done flag.
	 *  If tuple.done is true, the stream will end.
	 * @param {*} seed seed value
	 * @returns {Stream} stream containing all value of all tuples produced by the
	 *  unfolding function.
	 */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Compute a stream by iteratively calling f to produce values
	 * Event times may be controlled by returning a Promise from f
	 * @param {function(x:*):*|Promise<*>} f
	 * @param {*} x initial value
	 * @returns {Stream}
	 */

	/** @license MIT License (c) copyright 2010-2014 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Compute a stream using an *async* generator, which yields promises
	 * to control event times.
	 * @param f
	 * @returns {Stream}
	 */

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function continueWith (f, stream) {
	  return new Stream(new ContinueWith(f, stream.source))
	}

	function ContinueWith (f, source) {
	  this.f = f;
	  this.source = source;
	}

	ContinueWith.prototype.run = function (sink, scheduler) {
	  return new ContinueWithSink(this.f, this.source, sink, scheduler)
	};

	function ContinueWithSink (f, source, sink, scheduler) {
	  this.f = f;
	  this.sink = sink;
	  this.scheduler = scheduler;
	  this.active = true;
	  this.disposable = once(source.run(this, scheduler));
	}

	ContinueWithSink.prototype.error = Pipe.prototype.error;

	ContinueWithSink.prototype.event = function (t, x) {
	  if (!this.active) {
	    return
	  }
	  this.sink.event(t, x);
	};

	ContinueWithSink.prototype.end = function (t, x) {
	  if (!this.active) {
	    return
	  }

	  tryDispose(t, this.disposable, this.sink);
	  this._startNext(t, x, this.sink);
	};

	ContinueWithSink.prototype._startNext = function (t, x, sink) {
	  try {
	    this.disposable = this._continue(this.f, x, sink);
	  } catch (e) {
	    sink.error(t, e);
	  }
	};

	ContinueWithSink.prototype._continue = function (f, x, sink) {
	  return f(x).source.run(sink, this.scheduler)
	};

	ContinueWithSink.prototype.dispose = function () {
	  this.active = false;
	  return this.disposable.dispose()
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * @param {*} x value to prepend
	 * @param {Stream} stream
	 * @returns {Stream} new stream with x prepended
	 */
	function cons$1 (x, stream) {
	  return concat(of(x), stream)
	}

	/**
	* @param {Stream} left
	* @param {Stream} right
	* @returns {Stream} new stream containing all events in left followed by all
	*  events in right.  This *timeshifts* right to the end of left.
	*/
	function concat (left, right) {
	  return continueWith(function () {
	    return right
	  }, left)
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function IndexSink (i, sink) {
	  this.sink = sink;
	  this.index = i;
	  this.active = true;
	  this.value = void 0;
	}

	IndexSink.prototype.event = function (t, x) {
	  if (!this.active) {
	    return
	  }
	  this.value = x;
	  this.sink.event(t, this);
	};

	IndexSink.prototype.end = function (t, x) {
	  if (!this.active) {
	    return
	  }
	  this.active = false;
	  this.sink.end(t, { index: this.index, value: x });
	};

	IndexSink.prototype.error = Pipe.prototype.error;

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function invoke (f, args) {
	  /* eslint complexity: [2,7] */
	  switch (args.length) {
	    case 0: return f()
	    case 1: return f(args[0])
	    case 2: return f(args[0], args[1])
	    case 3: return f(args[0], args[1], args[2])
	    case 4: return f(args[0], args[1], args[2], args[3])
	    case 5: return f(args[0], args[1], args[2], args[3], args[4])
	    default:
	      return f.apply(void 0, args)
	  }
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	var map$3 = map;
	var tail$1 = tail;

	/**
	 * Combine latest events from all input streams
	 * @param {function(...events):*} f function to combine most recent events
	 * @returns {Stream} stream containing the result of applying f to the most recent
	 *  event of each input stream, whenever a new event arrives on any stream.
	 */
	function combine (f /*, ...streams */) {
	  return combineArray(f, tail$1(arguments))
	}

	/**
	* Combine latest events from all input streams
	* @param {function(...events):*} f function to combine most recent events
	* @param {[Stream]} streams most recent events
	* @returns {Stream} stream containing the result of applying f to the most recent
	*  event of each input stream, whenever a new event arrives on any stream.
	*/
	function combineArray (f, streams) {
	  var l = streams.length;
	  return l === 0 ? empty()
	  : l === 1 ? map$2(f, streams[0])
	  : new Stream(combineSources(f, streams))
	}

	function combineSources (f, streams) {
	  return new Combine(f, map$3(getSource, streams))
	}

	function getSource (stream) {
	  return stream.source
	}

	function Combine (f, sources) {
	  this.f = f;
	  this.sources = sources;
	}

	Combine.prototype.run = function (sink, scheduler) {
	  var l = this.sources.length;
	  var disposables = new Array(l);
	  var sinks = new Array(l);

	  var mergeSink = new CombineSink(disposables, sinks, sink, this.f);

	  for (var indexSink, i = 0; i < l; ++i) {
	    indexSink = sinks[i] = new IndexSink(i, mergeSink);
	    disposables[i] = this.sources[i].run(indexSink, scheduler);
	  }

	  return all(disposables)
	};

	function CombineSink (disposables, sinks, sink, f) {
	  this.sink = sink;
	  this.disposables = disposables;
	  this.sinks = sinks;
	  this.f = f;

	  var l = sinks.length;
	  this.awaiting = l;
	  this.values = new Array(l);
	  this.hasValue = new Array(l);
	  for (var i = 0; i < l; ++i) {
	    this.hasValue[i] = false;
	  }

	  this.activeCount = sinks.length;
	}

	CombineSink.prototype.error = Pipe.prototype.error;

	CombineSink.prototype.event = function (t, indexedValue) {
	  var i = indexedValue.index;
	  var awaiting = this._updateReady(i);

	  this.values[i] = indexedValue.value;
	  if (awaiting === 0) {
	    this.sink.event(t, invoke(this.f, this.values));
	  }
	};

	CombineSink.prototype._updateReady = function (index) {
	  if (this.awaiting > 0) {
	    if (!this.hasValue[index]) {
	      this.hasValue[index] = true;
	      this.awaiting -= 1;
	    }
	  }
	  return this.awaiting
	};

	CombineSink.prototype.end = function (t, indexedValue) {
	  tryDispose(t, this.disposables[indexedValue.index], this.sink);
	  if (--this.activeCount === 0) {
	    this.sink.end(t, indexedValue.value);
	  }
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Assume fs is a stream containing functions, and apply the latest function
	 * in fs to the latest value in xs.
	 * fs:         --f---------g--------h------>
	 * xs:         -a-------b-------c-------d-->
	 * ap(fs, xs): --fa-----fb-gb---gc--hc--hd->
	 * @param {Stream} fs stream of functions to apply to the latest x
	 * @param {Stream} xs stream of values to which to apply all the latest f
	 * @returns {Stream} stream containing all the applications of fs to xs
	 */
	function ap (fs, xs) {
	  return combine(apply, fs, xs)
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Transform a stream by passing its events through a transducer.
	 * @param  {function} transducer transducer function
	 * @param  {Stream} stream stream whose events will be passed through the
	 *  transducer
	 * @return {Stream} stream of events transformed by the transducer
	 */
	function transduce (transducer, stream) {
	  return new Stream(new Transduce(transducer, stream.source))
	}

	function Transduce (transducer, source) {
	  this.transducer = transducer;
	  this.source = source;
	}

	Transduce.prototype.run = function (sink, scheduler) {
	  var xf = this.transducer(new Transformer(sink));
	  return this.source.run(new TransduceSink(getTxHandler(xf), sink), scheduler)
	};

	function TransduceSink (adapter, sink) {
	  this.xf = adapter;
	  this.sink = sink;
	}

	TransduceSink.prototype.event = function (t, x) {
	  var next = this.xf.step(t, x);

	  return this.xf.isReduced(next)
	    ? this.sink.end(t, this.xf.getResult(next))
	    : next
	};

	TransduceSink.prototype.end = function (t, x) {
	  return this.xf.result(x)
	};

	TransduceSink.prototype.error = function (t, e) {
	  return this.sink.error(t, e)
	};

	function Transformer (sink) {
	  this.time = -Infinity;
	  this.sink = sink;
	}

	Transformer.prototype['@@transducer/init'] = Transformer.prototype.init = function () {};

	Transformer.prototype['@@transducer/step'] = Transformer.prototype.step = function (t, x) {
	  if (!isNaN(t)) {
	    this.time = Math.max(t, this.time);
	  }
	  return this.sink.event(this.time, x)
	};

	Transformer.prototype['@@transducer/result'] = Transformer.prototype.result = function (x) {
	  return this.sink.end(this.time, x)
	};

	/**
	* Given an object supporting the new or legacy transducer protocol,
	* create an adapter for it.
	* @param {object} tx transform
	* @returns {TxAdapter|LegacyTxAdapter}
	*/
	function getTxHandler (tx) {
	  return typeof tx['@@transducer/step'] === 'function'
	    ? new TxAdapter(tx)
	    : new LegacyTxAdapter(tx)
	}

	/**
	* Adapter for new official transducer protocol
	* @param {object} tx transform
	* @constructor
	*/
	function TxAdapter (tx) {
	  this.tx = tx;
	}

	TxAdapter.prototype.step = function (t, x) {
	  return this.tx['@@transducer/step'](t, x)
	};
	TxAdapter.prototype.result = function (x) {
	  return this.tx['@@transducer/result'](x)
	};
	TxAdapter.prototype.isReduced = function (x) {
	  return x != null && x['@@transducer/reduced']
	};
	TxAdapter.prototype.getResult = function (x) {
	  return x['@@transducer/value']
	};

	/**
	* Adapter for older transducer protocol
	* @param {object} tx transform
	* @constructor
	*/
	function LegacyTxAdapter (tx) {
	  this.tx = tx;
	}

	LegacyTxAdapter.prototype.step = function (t, x) {
	  return this.tx.step(t, x)
	};
	LegacyTxAdapter.prototype.result = function (x) {
	  return this.tx.result(x)
	};
	LegacyTxAdapter.prototype.isReduced = function (x) {
	  return x != null && x.__transducers_reduced__
	};
	LegacyTxAdapter.prototype.getResult = function (x) {
	  return x.value
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Doubly linked list
	 * @constructor
	 */
	function LinkedList () {
	  this.head = null;
	  this.length = 0;
	}

	/**
	 * Add a node to the end of the list
	 * @param {{prev:Object|null, next:Object|null, dispose:function}} x node to add
	 */
	LinkedList.prototype.add = function (x) {
	  if (this.head !== null) {
	    this.head.prev = x;
	    x.next = this.head;
	  }
	  this.head = x;
	  ++this.length;
	};

	/**
	 * Remove the provided node from the list
	 * @param {{prev:Object|null, next:Object|null, dispose:function}} x node to remove
	 */
	LinkedList.prototype.remove = function (x) { // eslint-disable-line  complexity
	  --this.length;
	  if (x === this.head) {
	    this.head = this.head.next;
	  }
	  if (x.next !== null) {
	    x.next.prev = x.prev;
	    x.next = null;
	  }
	  if (x.prev !== null) {
	    x.prev.next = x.next;
	    x.prev = null;
	  }
	};

	/**
	 * @returns {boolean} true iff there are no nodes in the list
	 */
	LinkedList.prototype.isEmpty = function () {
	  return this.length === 0
	};

	/**
	 * Dispose all nodes
	 * @returns {Promise} promise that fulfills when all nodes have been disposed,
	 *  or rejects if an error occurs while disposing
	 */
	LinkedList.prototype.dispose = function () {
	  if (this.isEmpty()) {
	    return Promise.resolve()
	  }

	  var promises = [];
	  var x = this.head;
	  this.head = null;
	  this.length = 0;

	  while (x !== null) {
	    promises.push(x.dispose());
	    x = x.next;
	  }

	  return Promise.all(promises)
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function mergeConcurrently (concurrency, stream) {
	  return mergeMapConcurrently(id, concurrency, stream)
	}

	function mergeMapConcurrently (f, concurrency, stream) {
	  return new Stream(new MergeConcurrently(f, concurrency, stream.source))
	}

	function MergeConcurrently (f, concurrency, source) {
	  this.f = f;
	  this.concurrency = concurrency;
	  this.source = source;
	}

	MergeConcurrently.prototype.run = function (sink, scheduler) {
	  return new Outer(this.f, this.concurrency, this.source, sink, scheduler)
	};

	function Outer (f, concurrency, source, sink, scheduler) {
	  this.f = f;
	  this.concurrency = concurrency;
	  this.sink = sink;
	  this.scheduler = scheduler;
	  this.pending = [];
	  this.current = new LinkedList();
	  this.disposable = once(source.run(this, scheduler));
	  this.active = true;
	}

	Outer.prototype.event = function (t, x) {
	  this._addInner(t, x);
	};

	Outer.prototype._addInner = function (t, x) {
	  if (this.current.length < this.concurrency) {
	    this._startInner(t, x);
	  } else {
	    this.pending.push(x);
	  }
	};

	Outer.prototype._startInner = function (t, x) {
	  try {
	    this._initInner(t, x);
	  } catch (e) {
	    this.error(t, e);
	  }
	};

	Outer.prototype._initInner = function (t, x) {
	  var innerSink = new Inner(t, this, this.sink);
	  innerSink.disposable = mapAndRun(this.f, x, innerSink, this.scheduler);
	  this.current.add(innerSink);
	};

	function mapAndRun (f, x, sink, scheduler) {
	  return f(x).source.run(sink, scheduler)
	}

	Outer.prototype.end = function (t, x) {
	  this.active = false;
	  tryDispose(t, this.disposable, this.sink);
	  this._checkEnd(t, x);
	};

	Outer.prototype.error = function (t, e) {
	  this.active = false;
	  this.sink.error(t, e);
	};

	Outer.prototype.dispose = function () {
	  this.active = false;
	  this.pending.length = 0;
	  return Promise.all([this.disposable.dispose(), this.current.dispose()])
	};

	Outer.prototype._endInner = function (t, x, inner) {
	  this.current.remove(inner);
	  tryDispose(t, inner, this);

	  if (this.pending.length === 0) {
	    this._checkEnd(t, x);
	  } else {
	    this._startInner(t, this.pending.shift());
	  }
	};

	Outer.prototype._checkEnd = function (t, x) {
	  if (!this.active && this.current.isEmpty()) {
	    this.sink.end(t, x);
	  }
	};

	function Inner (time, outer, sink) {
	  this.prev = this.next = null;
	  this.time = time;
	  this.outer = outer;
	  this.sink = sink;
	  this.disposable = void 0;
	}

	Inner.prototype.event = function (t, x) {
	  this.sink.event(Math.max(t, this.time), x);
	};

	Inner.prototype.end = function (t, x) {
	  this.outer._endInner(Math.max(t, this.time), x, this);
	};

	Inner.prototype.error = function (t, e) {
	  this.outer.error(Math.max(t, this.time), e);
	};

	Inner.prototype.dispose = function () {
	  return this.disposable.dispose()
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Map each value in the stream to a new stream, and merge it into the
	 * returned outer stream. Event arrival times are preserved.
	 * @param {function(x:*):Stream} f chaining function, must return a Stream
	 * @param {Stream} stream
	 * @returns {Stream} new stream containing all events from each stream returned by f
	 */
	function flatMap (f, stream) {
	  return mergeMapConcurrently(f, Infinity, stream)
	}

	/**
	 * Monadic join. Flatten a Stream<Stream<X>> to Stream<X> by merging inner
	 * streams to the outer. Event arrival times are preserved.
	 * @param {Stream<Stream<X>>} stream stream of streams
	 * @returns {Stream<X>} new stream containing all events of all inner streams
	 */
	function join (stream) {
	  return mergeConcurrently(Infinity, stream)
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Map each value in stream to a new stream, and concatenate them all
	 * stream:              -a---b---cX
	 * f(a):                 1-1-1-1X
	 * f(b):                        -2-2-2-2X
	 * f(c):                                -3-3-3-3X
	 * stream.concatMap(f): -1-1-1-1-2-2-2-2-3-3-3-3X
	 * @param {function(x:*):Stream} f function to map each value to a stream
	 * @param {Stream} stream
	 * @returns {Stream} new stream containing all events from each stream returned by f
	 */
	function concatMap (f, stream) {
	  return mergeMapConcurrently(f, 1, stream)
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	var copy$1 = copy;
	var reduce$2 = reduce;

	/**
	 * @returns {Stream} stream containing events from all streams in the argument
	 * list in time order.  If two events are simultaneous they will be merged in
	 * arbitrary order.
	 */
	function merge (/* ...streams */) {
	  return mergeArray(copy$1(arguments))
	}

	/**
	 * @param {Array} streams array of stream to merge
	 * @returns {Stream} stream containing events from all input observables
	 * in time order.  If two events are simultaneous they will be merged in
	 * arbitrary order.
	 */
	function mergeArray (streams) {
	  var l = streams.length;
	  return l === 0 ? empty()
	    : l === 1 ? streams[0]
	    : new Stream(mergeSources(streams))
	}

	/**
	 * This implements fusion/flattening for merge.  It will
	 * fuse adjacent merge operations.  For example:
	 * - a.merge(b).merge(c) effectively becomes merge(a, b, c)
	 * - merge(a, merge(b, c)) effectively becomes merge(a, b, c)
	 * It does this by concatenating the sources arrays of
	 * any nested Merge sources, in effect "flattening" nested
	 * merge operations into a single merge.
	 */
	function mergeSources (streams) {
	  return new Merge(reduce$2(appendSources, [], streams))
	}

	function appendSources (sources, stream) {
	  var source = stream.source;
	  return source instanceof Merge
	    ? sources.concat(source.sources)
	    : sources.concat(source)
	}

	function Merge (sources) {
	  this.sources = sources;
	}

	Merge.prototype.run = function (sink, scheduler) {
	  var l = this.sources.length;
	  var disposables = new Array(l);
	  var sinks = new Array(l);

	  var mergeSink = new MergeSink(disposables, sinks, sink);

	  for (var indexSink, i = 0; i < l; ++i) {
	    indexSink = sinks[i] = new IndexSink(i, mergeSink);
	    disposables[i] = this.sources[i].run(indexSink, scheduler);
	  }

	  return all(disposables)
	};

	function MergeSink (disposables, sinks, sink) {
	  this.sink = sink;
	  this.disposables = disposables;
	  this.activeCount = sinks.length;
	}

	MergeSink.prototype.error = Pipe.prototype.error;

	MergeSink.prototype.event = function (t, indexValue) {
	  this.sink.event(t, indexValue.value);
	};

	MergeSink.prototype.end = function (t, indexedValue) {
	  tryDispose(t, this.disposables[indexedValue.index], this.sink);
	  if (--this.activeCount === 0) {
	    this.sink.end(t, indexedValue.value);
	  }
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * When an event arrives on sampler, emit the result of calling f with the latest
	 * values of all streams being sampled
	 * @param {function(...values):*} f function to apply to each set of sampled values
	 * @param {Stream} sampler streams will be sampled whenever an event arrives
	 *  on sampler
	 * @returns {Stream} stream of sampled and transformed values
	 */


	/**
	 * When an event arrives on sampler, emit the latest event value from stream.
	 * @param {Stream} sampler stream of events at whose arrival time
	 *  stream's latest value will be propagated
	 * @param {Stream} stream stream of values
	 * @returns {Stream} sampled stream of values
	 */
	function sampleWith (sampler, stream) {
	  return new Stream(new Sampler(id, sampler.source, [stream.source]))
	}

	function sampleArray (f, sampler, streams) {
	  return new Stream(new Sampler(f, sampler.source, map(getSource$1, streams)))
	}

	function getSource$1 (stream) {
	  return stream.source
	}

	function Sampler (f, sampler, sources) {
	  this.f = f;
	  this.sampler = sampler;
	  this.sources = sources;
	}

	Sampler.prototype.run = function (sink, scheduler) {
	  var l = this.sources.length;
	  var disposables = new Array(l + 1);
	  var sinks = new Array(l);

	  var sampleSink = new SampleSink(this.f, sinks, sink);

	  for (var hold, i = 0; i < l; ++i) {
	    hold = sinks[i] = new Hold(sampleSink);
	    disposables[i] = this.sources[i].run(hold, scheduler);
	  }

	  disposables[i] = this.sampler.run(sampleSink, scheduler);

	  return all(disposables)
	};

	function Hold (sink) {
	  this.sink = sink;
	  this.hasValue = false;
	}

	Hold.prototype.event = function (t, x) {
	  this.value = x;
	  this.hasValue = true;
	  this.sink._notify(this);
	};

	Hold.prototype.end = function () {};
	Hold.prototype.error = Pipe.prototype.error;

	function SampleSink (f, sinks, sink) {
	  this.f = f;
	  this.sinks = sinks;
	  this.sink = sink;
	  this.active = false;
	}

	SampleSink.prototype._notify = function () {
	  if (!this.active) {
	    this.active = this.sinks.every(hasValue);
	  }
	};

	SampleSink.prototype.event = function (t) {
	  if (this.active) {
	    this.sink.event(t, invoke(this.f, map(getValue, this.sinks)));
	  }
	};

	SampleSink.prototype.end = Pipe.prototype.end;
	SampleSink.prototype.error = Pipe.prototype.error;

	function hasValue (hold) {
	  return hold.hasValue
	}

	function getValue (hold) {
	  return hold.value
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	// Based on https://github.com/petkaantonov/deque

	function Queue (capPow2) {
	  this._capacity = capPow2 || 32;
	  this._length = 0;
	  this._head = 0;
	}

	Queue.prototype.push = function (x) {
	  var len = this._length;
	  this._checkCapacity(len + 1);

	  var i = (this._head + len) & (this._capacity - 1);
	  this[i] = x;
	  this._length = len + 1;
	};

	Queue.prototype.shift = function () {
	  var head = this._head;
	  var x = this[head];

	  this[head] = void 0;
	  this._head = (head + 1) & (this._capacity - 1);
	  this._length--;
	  return x
	};

	Queue.prototype.isEmpty = function () {
	  return this._length === 0
	};

	Queue.prototype.length = function () {
	  return this._length
	};

	Queue.prototype._checkCapacity = function (size) {
	  if (this._capacity < size) {
	    this._ensureCapacity(this._capacity << 1);
	  }
	};

	Queue.prototype._ensureCapacity = function (capacity) {
	  var oldCapacity = this._capacity;
	  this._capacity = capacity;

	  var last = this._head + this._length;

	  if (last > oldCapacity) {
	    copy$2(this, 0, this, oldCapacity, last & (oldCapacity - 1));
	  }
	};

	function copy$2 (src, srcIndex, dst, dstIndex, len) {
	  for (var j = 0; j < len; ++j) {
	    dst[j + dstIndex] = src[j + srcIndex];
	    src[j + srcIndex] = void 0;
	  }
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	var map$4 = map;
	/**
	 * Combine streams pairwise (or tuple-wise) by index by applying f to values
	 * at corresponding indices.  The returned stream ends when any of the input
	 * streams ends.
	 * @param {function} f function to combine values
	 * @returns {Stream} new stream with items at corresponding indices combined
	 *  using f
	 */


	/**
	* Combine streams pairwise (or tuple-wise) by index by applying f to values
	* at corresponding indices.  The returned stream ends when any of the input
	* streams ends.
	* @param {function} f function to combine values
	* @param {[Stream]} streams streams to zip using f
	* @returns {Stream} new stream with items at corresponding indices combined
	*  using f
	*/
	function zipArray (f, streams) {
	  return streams.length === 0 ? empty()
	: streams.length === 1 ? map$2(f, streams[0])
	: new Stream(new Zip(f, map$4(getSource$2, streams)))
	}

	function getSource$2 (stream) {
	  return stream.source
	}

	function Zip (f, sources) {
	  this.f = f;
	  this.sources = sources;
	}

	Zip.prototype.run = function (sink, scheduler) {
	  var l = this.sources.length;
	  var disposables = new Array(l);
	  var sinks = new Array(l);
	  var buffers = new Array(l);

	  var zipSink = new ZipSink(this.f, buffers, sinks, sink);

	  for (var indexSink, i = 0; i < l; ++i) {
	    buffers[i] = new Queue();
	    indexSink = sinks[i] = new IndexSink(i, zipSink);
	    disposables[i] = this.sources[i].run(indexSink, scheduler);
	  }

	  return all(disposables)
	};

	function ZipSink (f, buffers, sinks, sink) {
	  this.f = f;
	  this.sinks = sinks;
	  this.sink = sink;
	  this.buffers = buffers;
	}

	ZipSink.prototype.event = function (t, indexedValue) { // eslint-disable-line complexity
	  var buffers = this.buffers;
	  var buffer = buffers[indexedValue.index];

	  buffer.push(indexedValue.value);

	  if (buffer.length() === 1) {
	    if (!ready(this.buffers)) {
	      return
	    }

	    emitZipped(this.f, t, buffers, this.sink);

	    if (ended(this.buffers, this.sinks)) {
	      this.sink.end(t, void 0);
	    }
	  }
	};

	ZipSink.prototype.end = function (t, indexedValue) {
	  var buffer = this.buffers[indexedValue.index];
	  if (buffer.isEmpty()) {
	    this.sink.end(t, indexedValue.value);
	  }
	};

	ZipSink.prototype.error = Pipe.prototype.error;

	function emitZipped (f, t, buffers, sink) {
	  sink.event(t, invoke(f, map$4(head, buffers)));
	}

	function head (buffer) {
	  return buffer.shift()
	}

	function ended (buffers, sinks) {
	  for (var i = 0, l = buffers.length; i < l; ++i) {
	    if (buffers[i].isEmpty() && !sinks[i].active) {
	      return true
	    }
	  }
	  return false
	}

	function ready (buffers) {
	  for (var i = 0, l = buffers.length; i < l; ++i) {
	    if (buffers[i].isEmpty()) {
	      return false
	    }
	  }
	  return true
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Given a stream of streams, return a new stream that adopts the behavior
	 * of the most recent inner stream.
	 * @param {Stream} stream of streams on which to switch
	 * @returns {Stream} switching stream
	 */
	function switchLatest (stream) {
	  return new Stream(new Switch(stream.source))
	}

	function Switch (source) {
	  this.source = source;
	}

	Switch.prototype.run = function (sink, scheduler) {
	  var switchSink = new SwitchSink(sink, scheduler);
	  return all([switchSink, this.source.run(switchSink, scheduler)])
	};

	function SwitchSink (sink, scheduler) {
	  this.sink = sink;
	  this.scheduler = scheduler;
	  this.current = null;
	  this.ended = false;
	}

	SwitchSink.prototype.event = function (t, stream) {
	  this._disposeCurrent(t); // TODO: capture the result of this dispose
	  this.current = new Segment(t, Infinity, this, this.sink);
	  this.current.disposable = stream.source.run(this.current, this.scheduler);
	};

	SwitchSink.prototype.end = function (t, x) {
	  this.ended = true;
	  this._checkEnd(t, x);
	};

	SwitchSink.prototype.error = function (t, e) {
	  this.ended = true;
	  this.sink.error(t, e);
	};

	SwitchSink.prototype.dispose = function () {
	  return this._disposeCurrent(this.scheduler.now())
	};

	SwitchSink.prototype._disposeCurrent = function (t) {
	  if (this.current !== null) {
	    return this.current._dispose(t)
	  }
	};

	SwitchSink.prototype._disposeInner = function (t, inner) {
	  inner._dispose(t); // TODO: capture the result of this dispose
	  if (inner === this.current) {
	    this.current = null;
	  }
	};

	SwitchSink.prototype._checkEnd = function (t, x) {
	  if (this.ended && this.current === null) {
	    this.sink.end(t, x);
	  }
	};

	SwitchSink.prototype._endInner = function (t, x, inner) {
	  this._disposeInner(t, inner);
	  this._checkEnd(t, x);
	};

	SwitchSink.prototype._errorInner = function (t, e, inner) {
	  this._disposeInner(t, inner);
	  this.sink.error(t, e);
	};

	function Segment (min, max, outer, sink) {
	  this.min = min;
	  this.max = max;
	  this.outer = outer;
	  this.sink = sink;
	  this.disposable = empty$1();
	}

	Segment.prototype.event = function (t, x) {
	  if (t < this.max) {
	    this.sink.event(Math.max(t, this.min), x);
	  }
	};

	Segment.prototype.end = function (t, x) {
	  this.outer._endInner(Math.max(t, this.min), x, this);
	};

	Segment.prototype.error = function (t, e) {
	  this.outer._errorInner(Math.max(t, this.min), e, this);
	};

	Segment.prototype._dispose = function (t) {
	  this.max = t;
	  tryDispose(t, this.disposable, this.sink);
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Retain only items matching a predicate
	 * @param {function(x:*):boolean} p filtering predicate called for each item
	 * @param {Stream} stream stream to filter
	 * @returns {Stream} stream containing only items for which predicate returns truthy
	 */
	function filter (p, stream) {
	  return new Stream(Filter.create(p, stream.source))
	}

	/**
	 * Skip repeated events, using === to detect duplicates
	 * @param {Stream} stream stream from which to omit repeated events
	 * @returns {Stream} stream without repeated events
	 */
	function skipRepeats (stream) {
	  return skipRepeatsWith(same, stream)
	}

	/**
	 * Skip repeated events using the provided equals function to detect duplicates
	 * @param {function(a:*, b:*):boolean} equals optional function to compare items
	 * @param {Stream} stream stream from which to omit repeated events
	 * @returns {Stream} stream without repeated events
	 */
	function skipRepeatsWith (equals, stream) {
	  return new Stream(new SkipRepeats(equals, stream.source))
	}

	function SkipRepeats (equals, source) {
	  this.equals = equals;
	  this.source = source;
	}

	SkipRepeats.prototype.run = function (sink, scheduler) {
	  return this.source.run(new SkipRepeatsSink(this.equals, sink), scheduler)
	};

	function SkipRepeatsSink (equals, sink) {
	  this.equals = equals;
	  this.sink = sink;
	  this.value = void 0;
	  this.init = true;
	}

	SkipRepeatsSink.prototype.end = Pipe.prototype.end;
	SkipRepeatsSink.prototype.error = Pipe.prototype.error;

	SkipRepeatsSink.prototype.event = function (t, x) {
	  if (this.init) {
	    this.init = false;
	    this.value = x;
	    this.sink.event(t, x);
	  } else if (!this.equals(this.value, x)) {
	    this.value = x;
	    this.sink.event(t, x);
	  }
	};

	function same (a, b) {
	  return a === b
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * @param {number} n
	 * @param {Stream} stream
	 * @returns {Stream} new stream containing only up to the first n items from stream
	 */
	function take (n, stream) {
	  return slice(0, n, stream)
	}

	/**
	 * @param {number} n
	 * @param {Stream} stream
	 * @returns {Stream} new stream with the first n items removed
	 */
	function skip (n, stream) {
	  return slice(n, Infinity, stream)
	}

	/**
	 * Slice a stream by index. Negative start/end indexes are not supported
	 * @param {number} start
	 * @param {number} end
	 * @param {Stream} stream
	 * @returns {Stream} stream containing items where start <= index < end
	 */
	function slice (start, end, stream) {
	  return end <= start ? empty()
	    : new Stream(sliceSource(start, end, stream.source))
	}

	function sliceSource (start, end, source) {
	  return source instanceof Map ? commuteMapSlice(start, end, source)
	    : source instanceof Slice ? fuseSlice(start, end, source)
	    : new Slice(start, end, source)
	}

	function commuteMapSlice (start, end, source) {
	  return Map.create(source.f, sliceSource(start, end, source.source))
	}

	function fuseSlice (start, end, source) {
	  start += source.min;
	  end = Math.min(end + source.min, source.max);
	  return new Slice(start, end, source.source)
	}

	function Slice (min, max, source) {
	  this.source = source;
	  this.min = min;
	  this.max = max;
	}

	Slice.prototype.run = function (sink, scheduler) {
	  var disposable = settable();
	  var sliceSink = new SliceSink(this.min, this.max - this.min, sink, disposable);

	  disposable.setDisposable(this.source.run(sliceSink, scheduler));
	  return disposable
	};

	function SliceSink (skip, take, sink, disposable) {
	  this.sink = sink;
	  this.skip = skip;
	  this.take = take;
	  this.disposable = disposable;
	}

	SliceSink.prototype.end = Pipe.prototype.end;
	SliceSink.prototype.error = Pipe.prototype.error;

	SliceSink.prototype.event = function (t, x) {
	  /* eslint complexity: [1, 4] */
	  if (this.skip > 0) {
	    this.skip -= 1;
	    return
	  }

	  if (this.take === 0) {
	    return
	  }

	  this.take -= 1;
	  this.sink.event(t, x);
	  if (this.take === 0) {
	    this.disposable.dispose();
	    this.sink.end(t, x);
	  }
	};

	function takeWhile (p, stream) {
	  return new Stream(new TakeWhile(p, stream.source))
	}

	function TakeWhile (p, source) {
	  this.p = p;
	  this.source = source;
	}

	TakeWhile.prototype.run = function (sink, scheduler) {
	  var disposable = settable();
	  var takeWhileSink = new TakeWhileSink(this.p, sink, disposable);

	  disposable.setDisposable(this.source.run(takeWhileSink, scheduler));
	  return disposable
	};

	function TakeWhileSink (p, sink, disposable) {
	  this.p = p;
	  this.sink = sink;
	  this.active = true;
	  this.disposable = disposable;
	}

	TakeWhileSink.prototype.end = Pipe.prototype.end;
	TakeWhileSink.prototype.error = Pipe.prototype.error;

	TakeWhileSink.prototype.event = function (t, x) {
	  if (!this.active) {
	    return
	  }

	  var p = this.p;
	  this.active = p(x);
	  if (this.active) {
	    this.sink.event(t, x);
	  } else {
	    this.disposable.dispose();
	    this.sink.end(t, x);
	  }
	};

	function skipWhile (p, stream) {
	  return new Stream(new SkipWhile(p, stream.source))
	}

	function SkipWhile (p, source) {
	  this.p = p;
	  this.source = source;
	}

	SkipWhile.prototype.run = function (sink, scheduler) {
	  return this.source.run(new SkipWhileSink(this.p, sink), scheduler)
	};

	function SkipWhileSink (p, sink) {
	  this.p = p;
	  this.sink = sink;
	  this.skipping = true;
	}

	SkipWhileSink.prototype.end = Pipe.prototype.end;
	SkipWhileSink.prototype.error = Pipe.prototype.error;

	SkipWhileSink.prototype.event = function (t, x) {
	  if (this.skipping) {
	    var p = this.p;
	    this.skipping = p(x);
	    if (this.skipping) {
	      return
	    }
	  }

	  this.sink.event(t, x);
	};

	function skipAfter (p, stream) {
	  return new Stream(new SkipAfter(p, stream.source))
	}

	function SkipAfter (p, source) {
	  this.p = p;
	  this.source = source;
	}

	SkipAfter.prototype.run = function run (sink, scheduler) {
	  return this.source.run(new SkipAfterSink(this.p, sink), scheduler)
	};

	function SkipAfterSink (p, sink) {
	  this.p = p;
	  this.sink = sink;
	  this.skipping = false;
	}

	SkipAfterSink.prototype.event = function event (t, x) {
	  if (this.skipping) {
	    return
	  }

	  var p = this.p;
	  this.skipping = p(x);
	  this.sink.event(t, x);

	  if (this.skipping) {
	    this.sink.end(t, x);
	  }
	};

	SkipAfterSink.prototype.end = Pipe.prototype.end;
	SkipAfterSink.prototype.error = Pipe.prototype.error;

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function takeUntil (signal, stream) {
	  return new Stream(new Until(signal.source, stream.source))
	}

	function skipUntil (signal, stream) {
	  return new Stream(new Since(signal.source, stream.source))
	}

	function during (timeWindow, stream) {
	  return takeUntil(join(timeWindow), skipUntil(timeWindow, stream))
	}

	function Until (maxSignal, source) {
	  this.maxSignal = maxSignal;
	  this.source = source;
	}

	Until.prototype.run = function (sink, scheduler) {
	  var min = new Bound(-Infinity, sink);
	  var max = new UpperBound(this.maxSignal, sink, scheduler);
	  var disposable = this.source.run(new TimeWindowSink(min, max, sink), scheduler);

	  return all([min, max, disposable])
	};

	function Since (minSignal, source) {
	  this.minSignal = minSignal;
	  this.source = source;
	}

	Since.prototype.run = function (sink, scheduler) {
	  var min = new LowerBound(this.minSignal, sink, scheduler);
	  var max = new Bound(Infinity, sink);
	  var disposable = this.source.run(new TimeWindowSink(min, max, sink), scheduler);

	  return all([min, max, disposable])
	};

	function Bound (value, sink) {
	  this.value = value;
	  this.sink = sink;
	}

	Bound.prototype.error = Pipe.prototype.error;
	Bound.prototype.event = noop;
	Bound.prototype.end = noop;
	Bound.prototype.dispose = noop;

	function TimeWindowSink (min, max, sink) {
	  this.min = min;
	  this.max = max;
	  this.sink = sink;
	}

	TimeWindowSink.prototype.event = function (t, x) {
	  if (t >= this.min.value && t < this.max.value) {
	    this.sink.event(t, x);
	  }
	};

	TimeWindowSink.prototype.error = Pipe.prototype.error;
	TimeWindowSink.prototype.end = Pipe.prototype.end;

	function LowerBound (signal, sink, scheduler) {
	  this.value = Infinity;
	  this.sink = sink;
	  this.disposable = signal.run(this, scheduler);
	}

	LowerBound.prototype.event = function (t /*, x */) {
	  if (t < this.value) {
	    this.value = t;
	  }
	};

	LowerBound.prototype.end = noop;
	LowerBound.prototype.error = Pipe.prototype.error;

	LowerBound.prototype.dispose = function () {
	  return this.disposable.dispose()
	};

	function UpperBound (signal, sink, scheduler) {
	  this.value = Infinity;
	  this.sink = sink;
	  this.disposable = signal.run(this, scheduler);
	}

	UpperBound.prototype.event = function (t, x) {
	  if (t < this.value) {
	    this.value = t;
	    this.sink.end(t, x);
	  }
	};

	UpperBound.prototype.end = noop;
	UpperBound.prototype.error = Pipe.prototype.error;

	UpperBound.prototype.dispose = function () {
	  return this.disposable.dispose()
	};

	function noop () {}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * @param {Number} delayTime milliseconds to delay each item
	 * @param {Stream} stream
	 * @returns {Stream} new stream containing the same items, but delayed by ms
	 */
	function delay (delayTime, stream) {
	  return delayTime <= 0 ? stream
	    : new Stream(new Delay(delayTime, stream.source))
	}

	function Delay (dt, source) {
	  this.dt = dt;
	  this.source = source;
	}

	Delay.prototype.run = function (sink, scheduler) {
	  var delaySink = new DelaySink(this.dt, sink, scheduler);
	  return all([delaySink, this.source.run(delaySink, scheduler)])
	};

	function DelaySink (dt, sink, scheduler) {
	  this.dt = dt;
	  this.sink = sink;
	  this.scheduler = scheduler;
	}

	DelaySink.prototype.dispose = function () {
	  var self = this;
	  this.scheduler.cancelAll(function (scheduledTask) {
	    return scheduledTask.task.sink === self.sink
	  });
	};

	DelaySink.prototype.event = function (t, x) {
	  this.scheduler.delay(this.dt, PropagateTask.event(x, this.sink));
	};

	DelaySink.prototype.end = function (t, x) {
	  this.scheduler.delay(this.dt, PropagateTask.end(x, this.sink));
	};

	DelaySink.prototype.error = Pipe.prototype.error;

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function timestamp (stream) {
	  return new Stream(new Timestamp(stream.source))
	}

	function Timestamp (source) {
	  this.source = source;
	}

	Timestamp.prototype.run = function (sink, scheduler) {
	  return this.source.run(new TimestampSink(sink), scheduler)
	};

	function TimestampSink (sink) {
	  this.sink = sink;
	}

	TimestampSink.prototype.end = Pipe.prototype.end;
	TimestampSink.prototype.error = Pipe.prototype.error;

	TimestampSink.prototype.event = function (t, x) {
	  this.sink.event(t, { time: t, value: x });
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Limit the rate of events by suppressing events that occur too often
	 * @param {Number} period time to suppress events
	 * @param {Stream} stream
	 * @returns {Stream}
	 */
	function throttle (period, stream) {
	  return new Stream(throttleSource(period, stream.source))
	}

	function throttleSource (period, source) {
	  return source instanceof Map ? commuteMapThrottle(period, source)
	    : source instanceof Throttle ? fuseThrottle(period, source)
	    : new Throttle(period, source)
	}

	function commuteMapThrottle (period, source) {
	  return Map.create(source.f, throttleSource(period, source.source))
	}

	function fuseThrottle (period, source) {
	  return new Throttle(Math.max(period, source.period), source.source)
	}

	function Throttle (period, source) {
	  this.period = period;
	  this.source = source;
	}

	Throttle.prototype.run = function (sink, scheduler) {
	  return this.source.run(new ThrottleSink(this.period, sink), scheduler)
	};

	function ThrottleSink (period, sink) {
	  this.time = 0;
	  this.period = period;
	  this.sink = sink;
	}

	ThrottleSink.prototype.event = function (t, x) {
	  if (t >= this.time) {
	    this.time = t + this.period;
	    this.sink.event(t, x);
	  }
	};

	ThrottleSink.prototype.end = Pipe.prototype.end;

	ThrottleSink.prototype.error = Pipe.prototype.error;

	/**
	 * Wait for a burst of events to subside and emit only the last event in the burst
	 * @param {Number} period events occuring more frequently than this
	 *  will be suppressed
	 * @param {Stream} stream stream to debounce
	 * @returns {Stream} new debounced stream
	 */
	function debounce (period, stream) {
	  return new Stream(new Debounce(period, stream.source))
	}

	function Debounce (dt, source) {
	  this.dt = dt;
	  this.source = source;
	}

	Debounce.prototype.run = function (sink, scheduler) {
	  return new DebounceSink(this.dt, this.source, sink, scheduler)
	};

	function DebounceSink (dt, source, sink, scheduler) {
	  this.dt = dt;
	  this.sink = sink;
	  this.scheduler = scheduler;
	  this.value = void 0;
	  this.timer = null;
	  this.disposable = source.run(this, scheduler);
	}

	DebounceSink.prototype.event = function (t, x) {
	  this._clearTimer();
	  this.value = x;
	  this.timer = this.scheduler.delay(this.dt, new DebounceTask(this, x));
	};

	DebounceSink.prototype._event = function (t, x) {
	  this._clearTimer();
	  this.sink.event(t, x);
	};

	DebounceSink.prototype.end = function (t, x) {
	  if (this._clearTimer()) {
	    this.sink.event(t, this.value);
	    this.value = void 0;
	  }
	  this.sink.end(t, x);
	};

	DebounceSink.prototype.error = function (t, x) {
	  this._clearTimer();
	  this.sink.error(t, x);
	};

	DebounceSink.prototype.dispose = function () {
	  this._clearTimer();
	  return this.disposable.dispose()
	};

	DebounceSink.prototype._clearTimer = function () {
	  if (this.timer === null) {
	    return false
	  }
	  this.timer.dispose();
	  this.timer = null;
	  return true
	};

	function DebounceTask (debounce, value) {
	  this.debounce = debounce;
	  this.value = value;
	}

	DebounceTask.prototype.run = function (t) {
	  this.debounce._event(t, this.value);
	};

	DebounceTask.prototype.error = function (t, e) {
	  this.debounce.error(t, e);
	};

	DebounceTask.prototype.dispose = function () {};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * Create a stream containing only the promise's fulfillment
	 * value at the time it fulfills.
	 * @param {Promise<T>} p promise
	 * @return {Stream<T>} stream containing promise's fulfillment value.
	 *  If the promise rejects, the stream will error
	 */
	function fromPromise (p) {
	  return awaitPromises(of(p))
	}

	/**
	 * Turn a Stream<Promise<T>> into Stream<T> by awaiting each promise.
	 * Event order is preserved.
	 * @param {Stream<Promise<T>>} stream
	 * @return {Stream<T>} stream of fulfillment values.  The stream will
	 * error if any promise rejects.
	 */
	function awaitPromises (stream) {
	  return new Stream(new Await(stream.source))
	}

	function Await (source) {
	  this.source = source;
	}

	Await.prototype.run = function (sink, scheduler) {
	  return this.source.run(new AwaitSink(sink, scheduler), scheduler)
	};

	function AwaitSink (sink, scheduler) {
	  this.sink = sink;
	  this.scheduler = scheduler;
	  this.queue = Promise.resolve();
	  var self = this;

	  // Pre-create closures, to avoid creating them per event
	  this._eventBound = function (x) {
	    self.sink.event(self.scheduler.now(), x);
	  };

	  this._endBound = function (x) {
	    self.sink.end(self.scheduler.now(), x);
	  };

	  this._errorBound = function (e) {
	    self.sink.error(self.scheduler.now(), e);
	  };
	}

	AwaitSink.prototype.event = function (t, promise) {
	  var self = this;
	  this.queue = this.queue.then(function () {
	    return self._event(promise)
	  }).catch(this._errorBound);
	};

	AwaitSink.prototype.end = function (t, x) {
	  var self = this;
	  this.queue = this.queue.then(function () {
	    return self._end(x)
	  }).catch(this._errorBound);
	};

	AwaitSink.prototype.error = function (t, e) {
	  var self = this;
	  // Don't resolve error values, propagate directly
	  this.queue = this.queue.then(function () {
	    return self._errorBound(e)
	  }).catch(fatalError);
	};

	AwaitSink.prototype._event = function (promise) {
	  return promise.then(this._eventBound)
	};

	AwaitSink.prototype._end = function (x) {
	  return Promise.resolve(x).then(this._endBound)
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	function SafeSink (sink) {
	  this.sink = sink;
	  this.active = true;
	}

	SafeSink.prototype.event = function (t, x) {
	  if (!this.active) {
	    return
	  }
	  this.sink.event(t, x);
	};

	SafeSink.prototype.end = function (t, x) {
	  if (!this.active) {
	    return
	  }
	  this.disable();
	  this.sink.end(t, x);
	};

	SafeSink.prototype.error = function (t, e) {
	  this.disable();
	  this.sink.error(t, e);
	};

	SafeSink.prototype.disable = function () {
	  this.active = false;
	  return this.sink
	};

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/**
	 * If stream encounters an error, recover and continue with items from stream
	 * returned by f.
	 * @param {function(error:*):Stream} f function which returns a new stream
	 * @param {Stream} stream
	 * @returns {Stream} new stream which will recover from an error by calling f
	 */
	function recoverWith (f, stream) {
	  return new Stream(new RecoverWith(f, stream.source))
	}

	var flatMapError = recoverWith;

	/**
	 * Create a stream containing only an error
	 * @param {*} e error value, preferably an Error or Error subtype
	 * @returns {Stream} new stream containing only an error
	 */


	function RecoverWith (f, source) {
	  this.f = f;
	  this.source = source;
	}

	RecoverWith.prototype.run = function (sink, scheduler) {
	  return new RecoverWithSink(this.f, this.source, sink, scheduler)
	};

	function RecoverWithSink (f, source, sink, scheduler) {
	  this.f = f;
	  this.sink = new SafeSink(sink);
	  this.scheduler = scheduler;
	  this.disposable = source.run(this, scheduler);
	}

	RecoverWithSink.prototype.event = function (t, x) {
	  tryEvent(t, x, this.sink);
	};

	RecoverWithSink.prototype.end = function (t, x) {
	  tryEnd(t, x, this.sink);
	};

	RecoverWithSink.prototype.error = function (t, e) {
	  var nextSink = this.sink.disable();

	  tryDispose(t, this.disposable, this.sink);
	  this._startNext(t, e, nextSink);
	};

	RecoverWithSink.prototype._startNext = function (t, x, sink) {
	  try {
	    this.disposable = this._continue(this.f, x, sink);
	  } catch (e) {
	    sink.error(t, e);
	  }
	};

	RecoverWithSink.prototype._continue = function (f, x, sink) {
	  var stream = f(x);
	  return stream.source.run(sink, this.scheduler)
	};

	RecoverWithSink.prototype.dispose = function () {
	  return this.disposable.dispose()
	};

	var MulticastDisposable = function MulticastDisposable (source, sink) {
	  this.source = source;
	  this.sink = sink;
	  this.disposed = false;
	};

	MulticastDisposable.prototype.dispose = function dispose () {
	  if (this.disposed) {
	    return
	  }
	  this.disposed = true;
	  var remaining = this.source.remove(this.sink);
	  return remaining === 0 && this.source._dispose()
	};

	function tryEvent$1 (t, x, sink) {
	  try {
	    sink.event(t, x);
	  } catch (e) {
	    sink.error(t, e);
	  }
	}

	function tryEnd$1 (t, x, sink) {
	  try {
	    sink.end(t, x);
	  } catch (e) {
	    sink.error(t, e);
	  }
	}

	var dispose = function (disposable) { return disposable.dispose(); };

	var emptyDisposable = {
	  dispose: function dispose$1 () {}
	};

	var MulticastSource = function MulticastSource (source) {
	  this.source = source;
	  this.sinks = [];
	  this._disposable = emptyDisposable;
	};

	MulticastSource.prototype.run = function run (sink, scheduler) {
	  var n = this.add(sink);
	  if (n === 1) {
	    this._disposable = this.source.run(this, scheduler);
	  }
	  return new MulticastDisposable(this, sink)
	};

	MulticastSource.prototype._dispose = function _dispose () {
	  var disposable = this._disposable;
	  this._disposable = emptyDisposable;
	  return Promise.resolve(disposable).then(dispose)
	};

	MulticastSource.prototype.add = function add (sink) {
	  this.sinks = append(sink, this.sinks);
	  return this.sinks.length
	};

	MulticastSource.prototype.remove = function remove$1 (sink) {
	  var i = findIndex(sink, this.sinks);
	  // istanbul ignore next
	  if (i >= 0) {
	    this.sinks = remove(i, this.sinks);
	  }

	  return this.sinks.length
	};

	MulticastSource.prototype.event = function event (time, value) {
	  var s = this.sinks;
	  if (s.length === 1) {
	    return s[0].event(time, value)
	  }
	  for (var i = 0; i < s.length; ++i) {
	    tryEvent$1(time, value, s[i]);
	  }
	};

	MulticastSource.prototype.end = function end (time, value) {
	  var s = this.sinks;
	  for (var i = 0; i < s.length; ++i) {
	    tryEnd$1(time, value, s[i]);
	  }
	};

	MulticastSource.prototype.error = function error (time, err) {
	  var s = this.sinks;
	  for (var i = 0; i < s.length; ++i) {
	    s[i].error(time, err);
	  }
	};

	function multicast (stream) {
	  var source = stream.source;
	  return source instanceof MulticastSource
	    ? stream
	    : new stream.constructor(new MulticastSource(source))
	}

	/** @license MIT License (c) copyright 2010-2016 original author or authors */
	/** @author Brian Cavalier */
	/** @author John Hann */

	/* eslint import/first: 0 */

	// Add of and empty to constructor for fantasy-land compat
	Stream.of = of;
	Stream.empty = empty;
	// Add from to constructor for ES Observable compat
	Stream.from = from;
	// -----------------------------------------------------------------------
	// Draft ES Observable proposal interop
	// https://github.com/zenparsing/es-observable

	Stream.prototype.subscribe = function (subscriber) {
	  return subscribe(subscriber, this)
	};

	Stream.prototype[result] = function () {
	  return this
	};

	// -----------------------------------------------------------------------
	// Fluent adapter

	/**
	 * Adapt a functional stream transform to fluent style.
	 * It applies f to the this stream object
	 * @param  {function(s: Stream): Stream} f function that
	 * receives the stream itself and must return a new stream
	 * @return {Stream}
	 */
	Stream.prototype.thru = function (f) {
	  return thru(f, this)
	};

	// -----------------------------------------------------------------------
	// Observing

	/**
	 * Process all the events in the stream
	 * @returns {Promise} promise that fulfills when the stream ends, or rejects
	 *  if the stream fails with an unhandled error.
	 */
	Stream.prototype.observe = Stream.prototype.forEach = function (f) {
	  return observe(f, this)
	};

	/**
	 * Consume all events in the stream, without providing a function to process each.
	 * This causes a stream to become active and begin emitting events, and is useful
	 * in cases where all processing has been setup upstream via other combinators, and
	 * there is no need to process the terminal events.
	 * @returns {Promise} promise that fulfills when the stream ends, or rejects
	 *  if the stream fails with an unhandled error.
	 */
	Stream.prototype.drain = function () {
	  return drain(this)
	};

	// -------------------------------------------------------

	/**
	 * Generalized feedback loop. Call a stepper function for each event. The stepper
	 * will be called with 2 params: the current seed and the an event value.  It must
	 * return a new { seed, value } pair. The `seed` will be fed back into the next
	 * invocation of stepper, and the `value` will be propagated as the event value.
	 * @param {function(seed:*, value:*):{seed:*, value:*}} stepper loop step function
	 * @param {*} seed initial seed value passed to first stepper call
	 * @returns {Stream} new stream whose values are the `value` field of the objects
	 * returned by the stepper
	 */
	Stream.prototype.loop = function (stepper, seed) {
	  return loop(stepper, seed, this)
	};

	// -------------------------------------------------------

	/**
	 * Create a stream containing successive reduce results of applying f to
	 * the previous reduce result and the current stream item.
	 * @param {function(result:*, x:*):*} f reducer function
	 * @param {*} initial initial value
	 * @returns {Stream} new stream containing successive reduce results
	 */
	Stream.prototype.scan = function (f, initial) {
	  return scan(f, initial, this)
	};

	/**
	 * Reduce the stream to produce a single result.  Note that reducing an infinite
	 * stream will return a Promise that never fulfills, but that may reject if an error
	 * occurs.
	 * @param {function(result:*, x:*):*} f reducer function
	 * @param {*} initial optional initial value
	 * @returns {Promise} promise for the file result of the reduce
	 */
	Stream.prototype.reduce = function (f, initial) {
	  return reduce$1(f, initial, this)
	};

	/**
	 * @param {Stream} tail
	 * @returns {Stream} new stream containing all items in this followed by
	 *  all items in tail
	 */
	Stream.prototype.concat = function (tail$$1) {
	  return concat(this, tail$$1)
	};

	/**
	 * @param {*} x value to prepend
	 * @returns {Stream} a new stream with x prepended
	 */
	Stream.prototype.startWith = function (x) {
	  return cons$1(x, this)
	};

	// -----------------------------------------------------------------------
	// Transforming

	/**
	 * Transform each value in the stream by applying f to each
	 * @param {function(*):*} f mapping function
	 * @returns {Stream} stream containing items transformed by f
	 */
	Stream.prototype.map = function (f) {
	  return map$2(f, this)
	};

	/**
	 * Assume this stream contains functions, and apply each function to each item
	 * in the provided stream.  This generates, in effect, a cross product.
	 * @param {Stream} xs stream of items to which
	 * @returns {Stream} stream containing the cross product of items
	 */
	Stream.prototype.ap = function (xs) {
	  return ap(this, xs)
	};

	/**
	 * Replace each value in the stream with x
	 * @param {*} x
	 * @returns {Stream} stream containing items replaced with x
	 */
	Stream.prototype.constant = function (x) {
	  return constant(x, this)
	};

	/**
	 * Perform a side effect for each item in the stream
	 * @param {function(x:*):*} f side effect to execute for each item. The
	 *  return value will be discarded.
	 * @returns {Stream} new stream containing the same items as this stream
	 */
	Stream.prototype.tap = function (f) {
	  return tap(f, this)
	};

	// -----------------------------------------------------------------------
	// Transducer support

	/**
	 * Transform this stream by passing its events through a transducer.
	 * @param  {function} transducer transducer function
	 * @return {Stream} stream of events transformed by the transducer
	 */
	Stream.prototype.transduce = function (transducer) {
	  return transduce(transducer, this)
	};

	// -----------------------------------------------------------------------
	// FlatMapping

	/**
	 * Map each value in the stream to a new stream, and merge it into the
	 * returned outer stream. Event arrival times are preserved.
	 * @param {function(x:*):Stream} f chaining function, must return a Stream
	 * @returns {Stream} new stream containing all events from each stream returned by f
	 */
	Stream.prototype.chain = function (f) {
	  return flatMap(f, this)
	};

	// @deprecated use chain instead
	Stream.prototype.flatMap = Stream.prototype.chain;

	  /**
	 * Monadic join. Flatten a Stream<Stream<X>> to Stream<X> by merging inner
	 * streams to the outer. Event arrival times are preserved.
	 * @returns {Stream<X>} new stream containing all events of all inner streams
	 */
	Stream.prototype.join = function () {
	  return join(this)
	};

	/**
	 * Map the end event to a new stream, and begin emitting its values.
	 * @param {function(x:*):Stream} f function that receives the end event value,
	 * and *must* return a new Stream to continue with.
	 * @returns {Stream} new stream that emits all events from the original stream,
	 * followed by all events from the stream returned by f.
	 */
	Stream.prototype.continueWith = function (f) {
	  return continueWith(f, this)
	};

	// @deprecated use continueWith instead
	Stream.prototype.flatMapEnd = Stream.prototype.continueWith;

	Stream.prototype.concatMap = function (f) {
	  return concatMap(f, this)
	};

	// -----------------------------------------------------------------------
	// Concurrent merging

	/**
	 * Flatten a Stream<Stream<X>> to Stream<X> by merging inner
	 * streams to the outer, limiting the number of inner streams that may
	 * be active concurrently.
	 * @param {number} concurrency at most this many inner streams will be
	 *  allowed to be active concurrently.
	 * @return {Stream<X>} new stream containing all events of all inner
	 *  streams, with limited concurrency.
	 */
	Stream.prototype.mergeConcurrently = function (concurrency) {
	  return mergeConcurrently(concurrency, this)
	};

	// -----------------------------------------------------------------------
	// Merging

	/**
	 * Merge this stream and all the provided streams
	 * @returns {Stream} stream containing items from this stream and s in time
	 * order.  If two events are simultaneous they will be merged in
	 * arbitrary order.
	 */
	Stream.prototype.merge = function (/* ...streams */) {
	  return mergeArray(cons(this, arguments))
	};

	// -----------------------------------------------------------------------
	// Combining

	/**
	 * Combine latest events from all input streams
	 * @param {function(...events):*} f function to combine most recent events
	 * @returns {Stream} stream containing the result of applying f to the most recent
	 *  event of each input stream, whenever a new event arrives on any stream.
	 */
	Stream.prototype.combine = function (f /*, ...streams */) {
	  return combineArray(f, replace(this, 0, arguments))
	};

	// -----------------------------------------------------------------------
	// Sampling

	/**
	 * When an event arrives on sampler, emit the latest event value from stream.
	 * @param {Stream} sampler stream of events at whose arrival time
	 *  signal's latest value will be propagated
	 * @returns {Stream} sampled stream of values
	 */
	Stream.prototype.sampleWith = function (sampler) {
	  return sampleWith(sampler, this)
	};

	/**
	 * When an event arrives on this stream, emit the result of calling f with the latest
	 * values of all streams being sampled
	 * @param {function(...values):*} f function to apply to each set of sampled values
	 * @returns {Stream} stream of sampled and transformed values
	 */
	Stream.prototype.sample = function (f /* ...streams */) {
	  return sampleArray(f, this, tail(arguments))
	};

	// -----------------------------------------------------------------------
	// Zipping

	/**
	 * Pair-wise combine items with those in s. Given 2 streams:
	 * [1,2,3] zipWith f [4,5,6] -> [f(1,4),f(2,5),f(3,6)]
	 * Note: zip causes fast streams to buffer and wait for slow streams.
	 * @param {function(a:Stream, b:Stream, ...):*} f function to combine items
	 * @returns {Stream} new stream containing pairs
	 */
	Stream.prototype.zip = function (f /*, ...streams */) {
	  return zipArray(f, replace(this, 0, arguments))
	};

	// -----------------------------------------------------------------------
	// Switching

	/**
	 * Given a stream of streams, return a new stream that adopts the behavior
	 * of the most recent inner stream.
	 * @returns {Stream} switching stream
	 */
	Stream.prototype.switchLatest = function () {
	  return switchLatest(this)
	};

	// @deprecated use switchLatest instead
	Stream.prototype.switch = Stream.prototype.switchLatest;

	// -----------------------------------------------------------------------
	// Filtering

	/**
	 * Retain only items matching a predicate
	 * stream:                           -12345678-
	 * filter(x => x % 2 === 0, stream): --2-4-6-8-
	 * @param {function(x:*):boolean} p filtering predicate called for each item
	 * @returns {Stream} stream containing only items for which predicate returns truthy
	 */
	Stream.prototype.filter = function (p) {
	  return filter(p, this)
	};

	/**
	 * Skip repeated events, using === to compare items
	 * stream:           -abbcd-
	 * distinct(stream): -ab-cd-
	 * @returns {Stream} stream with no repeated events
	 */
	Stream.prototype.skipRepeats = function () {
	  return skipRepeats(this)
	};

	/**
	 * Skip repeated events, using supplied equals function to compare items
	 * @param {function(a:*, b:*):boolean} equals function to compare items
	 * @returns {Stream} stream with no repeated events
	 */
	Stream.prototype.skipRepeatsWith = function (equals) {
	  return skipRepeatsWith(equals, this)
	};

	// -----------------------------------------------------------------------
	// Slicing

	/**
	 * stream:          -abcd-
	 * take(2, stream): -ab|
	 * @param {Number} n take up to this many events
	 * @returns {Stream} stream containing at most the first n items from this stream
	 */
	Stream.prototype.take = function (n) {
	  return take(n, this)
	};

	/**
	 * stream:          -abcd->
	 * skip(2, stream): ---cd->
	 * @param {Number} n skip this many events
	 * @returns {Stream} stream not containing the first n events
	 */
	Stream.prototype.skip = function (n) {
	  return skip(n, this)
	};

	/**
	 * Slice a stream by event index. Equivalent to, but more efficient than
	 * stream.take(end).skip(start);
	 * NOTE: Negative start and end are not supported
	 * @param {Number} start skip all events before the start index
	 * @param {Number} end allow all events from the start index to the end index
	 * @returns {Stream} stream containing items where start <= index < end
	 */
	Stream.prototype.slice = function (start, end) {
	  return slice(start, end, this)
	};

	/**
	 * stream:                        -123451234->
	 * takeWhile(x => x < 5, stream): -1234|
	 * @param {function(x:*):boolean} p predicate
	 * @returns {Stream} stream containing items up to, but not including, the
	 * first item for which p returns falsy.
	 */
	Stream.prototype.takeWhile = function (p) {
	  return takeWhile(p, this)
	};

	/**
	 * stream:                        -123451234->
	 * skipWhile(x => x < 5, stream): -----51234->
	 * @param {function(x:*):boolean} p predicate
	 * @returns {Stream} stream containing items following *and including* the
	 * first item for which p returns falsy.
	 */
	Stream.prototype.skipWhile = function (p) {
	  return skipWhile(p, this)
	};

	/**
	 * stream:                         -123456789->
	 * skipAfter(x => x === 5, stream):-12345|
	 * @param {function(x:*):boolean} p predicate
	 * @returns {Stream} stream containing items up to, *and including*, the
	 * first item for which p returns truthy.
	 */
	Stream.prototype.skipAfter = function (p) {
	  return skipAfter(p, this)
	};

	// -----------------------------------------------------------------------
	// Time slicing

	/**
	 * stream:                    -a-b-c-d-e-f-g->
	 * signal:                    -------x
	 * takeUntil(signal, stream): -a-b-c-|
	 * @param {Stream} signal retain only events in stream before the first
	 * event in signal
	 * @returns {Stream} new stream containing only events that occur before
	 * the first event in signal.
	 */
	Stream.prototype.until = function (signal) {
	  return takeUntil(signal, this)
	};

	// @deprecated use until instead
	Stream.prototype.takeUntil = Stream.prototype.until;

	  /**
	 * stream:                    -a-b-c-d-e-f-g->
	 * signal:                    -------x
	 * takeUntil(signal, stream): -------d-e-f-g->
	 * @param {Stream} signal retain only events in stream at or after the first
	 * event in signal
	 * @returns {Stream} new stream containing only events that occur after
	 * the first event in signal.
	 */
	Stream.prototype.since = function (signal) {
	  return skipUntil(signal, this)
	};

	// @deprecated use since instead
	Stream.prototype.skipUntil = Stream.prototype.since;

	  /**
	 * stream:                    -a-b-c-d-e-f-g->
	 * timeWindow:                -----s
	 * s:                               -----t
	 * stream.during(timeWindow): -----c-d-e-|
	 * @param {Stream<Stream>} timeWindow a stream whose first event (s) represents
	 *  the window start time.  That event (s) is itself a stream whose first event (t)
	 *  represents the window end time
	 * @returns {Stream} new stream containing only events within the provided timespan
	 */
	Stream.prototype.during = function (timeWindow) {
	  return during(timeWindow, this)
	};

	// -----------------------------------------------------------------------
	// Delaying

	/**
	 * @param {Number} delayTime milliseconds to delay each item
	 * @returns {Stream} new stream containing the same items, but delayed by ms
	 */
	Stream.prototype.delay = function (delayTime) {
	  return delay(delayTime, this)
	};

	// -----------------------------------------------------------------------
	// Getting event timestamp

	/**
	 * Expose event timestamps into the stream. Turns a Stream<X> into
	 * Stream<{time:t, value:X}>
	 * @returns {Stream<{time:number, value:*}>}
	 */
	Stream.prototype.timestamp = function () {
	  return timestamp(this)
	};

	// -----------------------------------------------------------------------
	// Rate limiting

	/**
	 * Limit the rate of events
	 * stream:              abcd----abcd----
	 * throttle(2, stream): a-c-----a-c-----
	 * @param {Number} period time to suppress events
	 * @returns {Stream} new stream that skips events for throttle period
	 */
	Stream.prototype.throttle = function (period) {
	  return throttle(period, this)
	};

	/**
	 * Wait for a burst of events to subside and emit only the last event in the burst
	 * stream:              abcd----abcd----
	 * debounce(2, stream): -----d-------d--
	 * @param {Number} period events occuring more frequently than this
	 *  on the provided scheduler will be suppressed
	 * @returns {Stream} new debounced stream
	 */
	Stream.prototype.debounce = function (period) {
	  return debounce(period, this)
	};

	// -----------------------------------------------------------------------
	// Awaiting Promises

	/**
	 * Await promises, turning a Stream<Promise<X>> into Stream<X>.  Preserves
	 * event order, but timeshifts events based on promise resolution time.
	 * @returns {Stream<X>} stream containing non-promise values
	 */
	Stream.prototype.awaitPromises = function () {
	  return awaitPromises(this)
	};

	// @deprecated use awaitPromises instead
	Stream.prototype.await = Stream.prototype.awaitPromises;

	// -----------------------------------------------------------------------
	// Error handling

	/**
	 * If this stream encounters an error, recover and continue with items from stream
	 * returned by f.
	 * stream:                  -a-b-c-X-
	 * f(X):                           d-e-f-g-
	 * flatMapError(f, stream): -a-b-c-d-e-f-g-
	 * @param {function(error:*):Stream} f function which returns a new stream
	 * @returns {Stream} new stream which will recover from an error by calling f
	 */
	Stream.prototype.recoverWith = function (f) {
	  return flatMapError(f, this)
	};

	// @deprecated use recoverWith instead
	Stream.prototype.flatMapError = Stream.prototype.recoverWith;

	// -----------------------------------------------------------------------
	// Multicasting

	/**
	 * Transform the stream into multicast stream.  That means that many subscribers
	 * to the stream will not cause multiple invocations of the internal machinery.
	 * @returns {Stream} new stream which will multicast events to all observers.
	 */
	Stream.prototype.multicast = function () {
	  return multicast(this)
	};

	// export the instance of the defaultScheduler for third-party libraries
	// export an implementation of Task used internally for third-party libraries

	var out_of_memory = /* tuple */[
	  "Out_of_memory",
	  0
	];

	var sys_error = /* tuple */[
	  "Sys_error",
	  -1
	];

	var failure = /* tuple */[
	  "Failure",
	  -2
	];

	var invalid_argument = /* tuple */[
	  "Invalid_argument",
	  -3
	];

	var end_of_file = /* tuple */[
	  "End_of_file",
	  -4
	];

	var division_by_zero = /* tuple */[
	  "Division_by_zero",
	  -5
	];

	var not_found = /* tuple */[
	  "Not_found",
	  -6
	];

	var match_failure = /* tuple */[
	  "Match_failure",
	  -7
	];

	var stack_overflow = /* tuple */[
	  "Stack_overflow",
	  -8
	];

	var sys_blocked_io = /* tuple */[
	  "Sys_blocked_io",
	  -9
	];

	var assert_failure = /* tuple */[
	  "Assert_failure",
	  -10
	];

	var undefined_recursive_module = /* tuple */[
	  "Undefined_recursive_module",
	  -11
	];

	out_of_memory.tag = 248;

	sys_error.tag = 248;

	failure.tag = 248;

	invalid_argument.tag = 248;

	end_of_file.tag = 248;

	division_by_zero.tag = 248;

	not_found.tag = 248;

	match_failure.tag = 248;

	stack_overflow.tag = 248;

	sys_blocked_io.tag = 248;

	assert_failure.tag = 248;

	undefined_recursive_module.tag = 248;


	/*  Not a pure module */

	function caml_array_sub(x, offset, len) {
	  var result = new Array(len);
	  var j = 0;
	  var i = offset;
	  while(j < len) {
	    result[j] = x[i];
	    j = j + 1 | 0;
	    i = i + 1 | 0;
	  }
	  return result;
	}

	function caml_array_get(xs, index) {
	  if (index < 0 || index >= xs.length) {
	    throw [
	          invalid_argument,
	          "index out of bounds"
	        ];
	  } else {
	    return xs[index];
	  }
	}


	/* No side effect */

	function app(_f, _args) {
	  while(true) {
	    var args = _args;
	    var f = _f;
	    var arity = f.length;
	    var len = args.length;
	    var d = arity - len | 0;
	    if (d === 0) {
	      return f.apply(null, args);
	    } else if (d < 0) {
	      _args = caml_array_sub(args, arity, -d | 0);
	      _f = f.apply(null, caml_array_sub(args, 0, arity));
	      continue ;
	    } else {
	      return (function(f,args){
	      return function (x) {
	        return app(f, args.concat(/* array */[x]));
	      }
	      }(f,args));
	    }
	  }
	}

	function curry_1(o, a0, arity) {
	  switch (arity) {
	    case 1 : 
	        return o(a0);
	    case 2 : 
	        return (function (param) {
	            return o(a0, param);
	          });
	    case 3 : 
	        return (function (param, param$1) {
	            return o(a0, param, param$1);
	          });
	    case 4 : 
	        return (function (param, param$1, param$2) {
	            return o(a0, param, param$1, param$2);
	          });
	    case 5 : 
	        return (function (param, param$1, param$2, param$3) {
	            return o(a0, param, param$1, param$2, param$3);
	          });
	    case 6 : 
	        return (function (param, param$1, param$2, param$3, param$4) {
	            return o(a0, param, param$1, param$2, param$3, param$4);
	          });
	    case 7 : 
	        return (function (param, param$1, param$2, param$3, param$4, param$5) {
	            return o(a0, param, param$1, param$2, param$3, param$4, param$5);
	          });
	    default:
	      return app(o, /* array */[a0]);
	  }
	}

	function _1(o, a0) {
	  var arity = o.length;
	  if (arity === 1) {
	    return o(a0);
	  } else {
	    return curry_1(o, a0, arity);
	  }
	}

	function curry_2(o, a0, a1, arity) {
	  switch (arity) {
	    case 1 : 
	        return app(o(a0), /* array */[a1]);
	    case 2 : 
	        return o(a0, a1);
	    case 3 : 
	        return (function (param) {
	            return o(a0, a1, param);
	          });
	    case 4 : 
	        return (function (param, param$1) {
	            return o(a0, a1, param, param$1);
	          });
	    case 5 : 
	        return (function (param, param$1, param$2) {
	            return o(a0, a1, param, param$1, param$2);
	          });
	    case 6 : 
	        return (function (param, param$1, param$2, param$3) {
	            return o(a0, a1, param, param$1, param$2, param$3);
	          });
	    case 7 : 
	        return (function (param, param$1, param$2, param$3, param$4) {
	            return o(a0, a1, param, param$1, param$2, param$3, param$4);
	          });
	    default:
	      return app(o, /* array */[
	                  a0,
	                  a1
	                ]);
	  }
	}

	function _2(o, a0, a1) {
	  var arity = o.length;
	  if (arity === 2) {
	    return o(a0, a1);
	  } else {
	    return curry_2(o, a0, a1, arity);
	  }
	}

	function curry_3(o, a0, a1, a2, arity) {
	  switch (arity) {
	    case 1 : 
	        return app(o(a0), /* array */[
	                    a1,
	                    a2
	                  ]);
	    case 2 : 
	        return app(o(a0, a1), /* array */[a2]);
	    case 3 : 
	        return o(a0, a1, a2);
	    case 4 : 
	        return (function (param) {
	            return o(a0, a1, a2, param);
	          });
	    case 5 : 
	        return (function (param, param$1) {
	            return o(a0, a1, a2, param, param$1);
	          });
	    case 6 : 
	        return (function (param, param$1, param$2) {
	            return o(a0, a1, a2, param, param$1, param$2);
	          });
	    case 7 : 
	        return (function (param, param$1, param$2, param$3) {
	            return o(a0, a1, a2, param, param$1, param$2, param$3);
	          });
	    default:
	      return app(o, /* array */[
	                  a0,
	                  a1,
	                  a2
	                ]);
	  }
	}

	function _3(o, a0, a1, a2) {
	  var arity = o.length;
	  if (arity === 3) {
	    return o(a0, a1, a2);
	  } else {
	    return curry_3(o, a0, a1, a2, arity);
	  }
	}


	/* No side effect */

	var id$1 = /* record */[/* contents */0];

	function caml_fresh_oo_id(param) {
	  id$1[0] += 1;
	  return id$1[0];
	}

	function create$1(str) {
	  var v_001 = caml_fresh_oo_id(/* () */0);
	  var v = /* tuple */[
	    str,
	    v_001
	  ];
	  v.tag = 248;
	  return v;
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	var Check_fail = create$1("Exception-WonderLog.Check_fail");

	var $$throw = (
	    function(msg){
	            throw new Error(msg)
	    });


	/* throw Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE








	function debug$1 (msg){
	    if (typeof window === "undefined" || typeof window.wonder_console === "undefined") {
	      return;
	    }

	    window.wonder_console.debug(msg);
	  }





	function trace (func){
	    if (typeof window === "undefined" || typeof window.wonder_console === "undefined") {
	      return;
	    }

	    window.wonder_console.trace(func);
	  }


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _debug(msg) {
	  console.log(msg);
	  return debug$1(msg);
	}

	function _trace(func) {
	  console.trace();
	  trace(func);
	  return /* () */0;
	}

	function buildDebugMessage(description, params, param) {
	  return "\n  Debug:\n\n  description\n  " + (String(description) + ("\n\n  params\n  " + (String(params) + "\n\n  ")));
	}

	function debugWithFunc(func, isTest) {
	  if (isTest) {
	    return _1(func, /* () */0);
	  } else {
	    return /* () */0;
	  }
	}

	function debug(buildMessageFunc, isTest) {
	  if (isTest) {
	    _debug(_1(buildMessageFunc, /* () */0));
	    return _trace(debug);
	  } else {
	    return /* () */0;
	  }
	}

	function buildFatalMessage(title, description, reason, solution, params) {
	  return "\n  Fatal:\n\n  title\n  " + (String(title) + ("\n\n  description\n  " + (String(description) + ("\n\n  reason\n  " + (String(reason) + ("\n\n  solution\n  " + (String(solution) + ("\n\n  params\n  " + (String(params) + "\n\n   ")))))))));
	}

	function fatal(msg) {
	  return _1($$throw, msg);
	}

	function buildErrorMessage(title, description, reason, solution, params) {
	  return "\n  Error:\n\n  title\n  " + (String(title) + ("\n\n  description\n  " + (String(description) + ("\n\n  reason\n  " + (String(reason) + ("\n\n  solution\n  " + (String(solution) + ("\n\n  params\n  " + (String(params) + "\n\n   ")))))))));
	}

	function buildAssertMessage(expect, actual) {
	  return "expect " + (String(expect) + (", but actual " + (String(actual) + "")));
	}


	/* Exception-WonderLog Not a pure module */

	/* No side effect */

	/* No side effect */

	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	var convertStringToInt = (
	function(str) {
	    return parseInt(str, 10);
	}
	);


	/* convertStringToInt Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function fetch$1(filePath) {
	  return fetch(filePath);
	}

	function _getContentLength (response){
	  return response.headers.get("content-length");
	  }

	function getContentLength(response) {
	  var match = _getContentLength(response);
	  if (match == null) {
	    return fatal(buildErrorMessage("load", "Content-Length response header unavailable", "", "", ""));
	  } else {
	    return _1(convertStringToInt, match);
	  }
	}


	/* most Not a pure module */

	var undefinedHeader = /* array */[];

	function some$1(x) {
	  if (x === undefined) {
	    var block = /* tuple */[
	      undefinedHeader,
	      0
	    ];
	    block.tag = 256;
	    return block;
	  } else if (x !== null && x[0] === undefinedHeader) {
	    var nid = x[1] + 1 | 0;
	    var block$1 = /* tuple */[
	      undefinedHeader,
	      nid
	    ];
	    block$1.tag = 256;
	    return block$1;
	  } else {
	    return x;
	  }
	}

	function nullable_to_opt(x) {
	  if (x === null || x === undefined) {
	    return undefined;
	  } else {
	    return some$1(x);
	  }
	}

	function valFromOption(x) {
	  if (x !== null && x[0] === undefinedHeader) {
	    var depth = x[1];
	    if (depth === 0) {
	      return undefined;
	    } else {
	      return /* tuple */[
	              undefinedHeader,
	              depth - 1 | 0
	            ];
	    }
	  } else {
	    return x;
	  }
	}


	/* No side effect */

	function isSome(param) {
	  return param !== undefined;
	}

	function getExn(x) {
	  if (x !== undefined) {
	    return valFromOption(x);
	  } else {
	    throw new Error("getExn");
	  }
	}


	/* No side effect */

	var $$Error = create$1("Caml_js_exceptions.Error");


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _assert(result, msg) {
	  if (result) {
	    return /* () */0;
	  } else {
	    throw [
	          Check_fail,
	          msg
	        ];
	  }
	}

	function test (message,func){
	  try{
	  func();
	  } catch(e){
	    throw new Error(JSON.stringify(message));
	  }
	  }



	function requireCheck(f, isTest) {
	  if (isTest) {
	    return _1(f, /* () */0);
	  } else {
	    return /* () */0;
	  }
	}

	function assertExist(source) {
	  return _assert(isSome(source), "expect exist, but actual not");
	}


	/* _isNullableExist Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function unsafeGet(optionData) {
	  requireCheck((function (param) {
	          return test(buildAssertMessage("data exist(get by getExn)", "not"), (function (param) {
	                        return assertExist(optionData);
	                      }));
	        }), true);
	  return getExn(optionData);
	}


	/* Log-WonderLog Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function unsafeGetFontData(record) {
	  return unsafeGet(record[/* fontData */2]);
	}

	function unsafeGetWebglData(record) {
	  return unsafeGet(record[/* webglData */3]);
	}

	function getSetting$1(record) {
	  return record[/* setting */0];
	}

	function setSetting$1(setting, record) {
	  return /* record */[
	          /* setting */setting,
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData */record[/* extendData */8]
	        ];
	}

	function getIOData(record) {
	  return record[/* ioData */6];
	}

	function getFontTextureDrawData(record) {
	  return record[/* drawData */4][/* fontTextureDrawData */0];
	}

	function getCustomTextureDrawDataMap(record) {
	  return record[/* drawData */4][/* customTextureDrawDataMap */1];
	}


	/* OptionService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function buildIOData$2($staropt$star, $staropt$star$1, $staropt$star$2, $staropt$star$3, param) {
	  var pointUp = $staropt$star !== undefined ? $staropt$star : false;
	  var pointDown = $staropt$star$1 !== undefined ? $staropt$star$1 : false;
	  var pointPosition = $staropt$star$2 !== undefined ? $staropt$star$2 : /* tuple */[
	      0,
	      0
	    ];
	  var pointMovementDelta = $staropt$star$3 !== undefined ? $staropt$star$3 : /* tuple */[
	      0,
	      0
	    ];
	  return {
	          pointUp: pointUp,
	          pointDown: pointDown,
	          pointPosition: pointPosition,
	          pointMovementDelta: pointMovementDelta
	        };
	}

	function isClick(record) {
	  var match = getIOData(record);
	  if (match[/* pointDown */1]) {
	    return match[/* pointUp */0];
	  } else {
	    return false;
	  }
	}


	/* RecordIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE


	function createObjectURL (blob){
	     return URL.createObjectURL( blob )
	    }

	function revokeObjectURL (blob){
	     URL.revokeObjectURL( blob );
	    }


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function extname(path) {
	  var match = (/\.(\w+)$/).exec(path);
	  if (match !== null) {
	    return nullable_to_opt(caml_array_get(match, 0));
	  }
	  
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function getType(src) {
	  var match = extname(src);
	  if (match !== undefined) {
	    switch (match) {
	      case ".jpg" : 
	          return /* Jpg */0;
	      case ".png" : 
	          return /* Png */1;
	      default:
	        return /* Unknown */2;
	    }
	  } else {
	    return fatal(buildFatalMessage("getType", "image extname should exist", "", "", ""));
	  }
	}

	function _loadBlobImage (objectUrl,resolve,reject){
	          if (typeof window.loadImageBlob_wonder_imgui === "undefined") {
	  window.loadImageBlob_wonder_imgui = function(objectUrl, resolve, reject){
	                      var image = new Image();

	                      image.src = objectUrl;

	                      image.onload = (function () {
	                          return resolve(image);
	                        });

	                      image.onerror = (function (e) {
	                        console.trace();
	                                return reject(new Error(e));
	                              });
	  };
	          }

	  window.loadImageBlob_wonder_imgui(objectUrl, resolve, reject);
	    }

	function loadImageByBlobPromise(objectUrl) {
	  return fromPromise(new Promise((function (resolve, reject) {
	                    return _loadBlobImage(objectUrl, resolve, reject);
	                  })));
	}


	/* most Not a pure module */

	var imul = ( Math.imul || function (x,y) {
	  y |= 0; return ((((x >> 16) * y) << 16) + (x & 0xffff) * y)|0; 
	}
	);


	/* imul Not a pure module */

	function iter(x, f) {
	  if (x == null) {
	    return /* () */0;
	  } else {
	    return f(x);
	  }
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE


	function reduceOneParam$1(func, param, arr) {
	  var mutableParam = param;
	  for(var i = 0 ,i_finish = arr.length - 1 | 0; i <= i_finish; ++i){
	    mutableParam = func(mutableParam, arr[i]);
	  }
	  return mutableParam;
	}


	/* No side effect */

	function entries$2(dict) {
	  var keys = Object.keys(dict);
	  var l = keys.length;
	  var values = new Array(l);
	  for(var i = 0 ,i_finish = l - 1 | 0; i <= i_finish; ++i){
	    var key = keys[i];
	    values[i] = /* tuple */[
	      key,
	      dict[key]
	    ];
	  }
	  return values;
	}

	function values(dict) {
	  var keys = Object.keys(dict);
	  var l = keys.length;
	  var values$1 = new Array(l);
	  for(var i = 0 ,i_finish = l - 1 | 0; i <= i_finish; ++i){
	    values$1[i] = dict[keys[i]];
	  }
	  return values$1;
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE


	function isEmpty(value) {
	  if (value === null) {
	    return true;
	  } else {
	    return value === undefined;
	  }
	}

	function isInMap(value) {
	  return value !== undefined;
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function createEmpty$2(param) {
	  return { };
	}

	function unsafeGet$2(key, map) {
	  return map[key];
	}

	function get$2(key, map) {
	  var value = map[key];
	  var match = isEmpty(value);
	  if (match) {
	    return undefined;
	  } else {
	    return some$1(value);
	  }
	}

	function getValidValues$1(map) {
	  return values(map).filter(isInMap);
	}

	function _mutableSet(key, value, map) {
	  map[key] = value;
	  return map;
	}

	function copy$4(map) {
	  return reduceOneParam$1((function (newMap, param) {
	                return _mutableSet(param[0], param[1], newMap);
	              }), { }, entries$2(map));
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function set(key, value, map) {
	  map[key] = value;
	  return map;
	}

	var createEmpty$1 = createEmpty$2;

	var get$1 = get$2;

	var unsafeGet$1 = unsafeGet$2;

	var getValidValues = getValidValues$1;


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function createEmpty(param) {
	  return /* array */[];
	}

	function forEach(func, arr) {
	  for(var i = 0 ,i_finish = arr.length - 1 | 0; i <= i_finish; ++i){
	    func(arr[i]);
	  }
	  return /* () */0;
	}

	function reduceOneParami(func, param, arr) {
	  var mutableParam = param;
	  for(var i = 0 ,i_finish = arr.length - 1 | 0; i <= i_finish; ++i){
	    mutableParam = func(mutableParam, arr[i], i);
	  }
	  return mutableParam;
	}

	var reduceOneParam = reduceOneParam$1;


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function range(a, b) {
	  var result = createEmpty(/* () */0);
	  for(var i = a; i <= b; ++i){
	    result.push(i);
	  }
	  return result;
	}

	function push(item, arr) {
	  arr.push(item);
	  return arr;
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE


	function toInt (str){
	    return parseInt(str);
	    }


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function createEmpty$4(param) {
	  return /* array */[];
	}

	function get$5(key, map) {
	  var value = map[key];
	  var match = isEmpty(value);
	  if (match) {
	    return undefined;
	  } else {
	    return some$1(value);
	  }
	}

	function length$3(prim) {
	  return prim.length;
	}

	function getValidValues$3(map) {
	  return map.filter(isInMap);
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function set$1(key, value, map) {
	  map[key] = value;
	  return map;
	}

	var createEmpty$3 = createEmpty$4;

	var get$4 = get$5;

	var length$2 = length$3;

	var getValidValues$2 = getValidValues$3;


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _parseStrToObj(str) {
	  var item_exp = (/\w+=[^ \r\n]+/gi);
	  var int_exp = (/^[\-]?\d+$/);
	  var obj = createEmpty$1(/* () */0);
	  var $$break = false;
	  while(!$$break) {
	    var match = item_exp.exec(str);
	    if (match !== null) {
	      iter(caml_array_get(match, 0), (function (tempStr) {
	              var index = tempStr.indexOf("=");
	              var key = tempStr.substring(0, index);
	              var value = tempStr.substring(index + 1 | 0);
	              var match = int_exp.test(value);
	              var value$1;
	              if (match) {
	                value$1 = value;
	              } else {
	                var match$1 = value[0] === "\"";
	                value$1 = match$1 ? value.substring(1, value.length) : value;
	              }
	              set(key, toInt(value$1), obj);
	              return /* () */0;
	            }));
	    } else {
	      $$break = true;
	    }
	  }
	  return obj;
	}

	function _parseChar(fntStr) {
	  var fontDefDictionary = createEmpty$3(/* () */0);
	  var char_exp = (/char [^\n]*(\n|$)/gi);
	  var $$break = false;
	  while(!$$break) {
	    var match = char_exp.exec(fntStr);
	    if (match !== null) {
	      iter(caml_array_get(match, 0), (function ($$char) {
	              var charObj = _parseStrToObj($$char);
	              var charId = unsafeGet$1("id", charObj);
	              set$1(charId, /* record */[
	                    /* id */charId,
	                    /* rect : tuple */[
	                      unsafeGet$1("x", charObj),
	                      unsafeGet$1("y", charObj),
	                      unsafeGet$1("width", charObj),
	                      unsafeGet$1("height", charObj)
	                    ],
	                    /* xOffset */unsafeGet$1("xoffset", charObj),
	                    /* yOffset */unsafeGet$1("yoffset", charObj),
	                    /* xAdvance */unsafeGet$1("xadvance", charObj)
	                  ], fontDefDictionary);
	              return /* () */0;
	            }));
	    } else {
	      $$break = true;
	    }
	  }
	  return fontDefDictionary;
	}

	function buildKerningHashMapKey(first, second) {
	  return imul(first, 1000) + second | 0;
	}

	function _changeKerningArrayToHashMap(kerningArray) {
	  return reduceOneParam((function (map, param) {
	                return set$1(buildKerningHashMapKey(param[/* first */0], param[/* second */1]), param[/* amount */2], map);
	              }), createEmpty$3(/* () */0), kerningArray);
	}

	function _parseKerning(fntStr) {
	  var kerningArray = /* array */[];
	  var kerning_exp = (/kerning [^\n]*(\n|$)/gi);
	  var $$break = false;
	  while(!$$break) {
	    var match = kerning_exp.exec(fntStr);
	    if (match !== null) {
	      iter(caml_array_get(match, 0), (function (kerning) {
	              var kerningObj = _parseStrToObj(kerning);
	              push(/* record */[
	                    /* first */unsafeGet$1("first", kerningObj),
	                    /* second */unsafeGet$1("second", kerningObj),
	                    /* amount */unsafeGet$1("amount", kerningObj)
	                  ], kerningArray);
	              return /* () */0;
	            }));
	    } else {
	      $$break = true;
	    }
	  }
	  return _changeKerningArrayToHashMap(kerningArray);
	}

	function parse(fntStr, url) {
	  var common_exp = (/common [^\n]*(\n|$)/gi);
	  var page_exp = (/page [^\n]*(\n|$)/gi);
	  var match = common_exp.exec(fntStr);
	  var commonObj = _parseStrToObj(match !== null ? unsafeGet(nullable_to_opt(caml_array_get(match, 0))) : fatal(buildFatalMessage("parse", "fntStr should match common_exp", "", "", "fntStr: " + (String(fntStr) + ("\n    common_exp: " + (String(common_exp) + " "))))));
	  var match$1 = page_exp.exec(fntStr);
	  var pageObj = _parseStrToObj(match$1 !== null ? unsafeGet(nullable_to_opt(caml_array_get(match$1, 0))) : fatal(buildFatalMessage("parse", "fntStr should match page_exp", "", "", "fntStr: " + (String(fntStr) + ("\n    page_exp: " + (String(page_exp) + " "))))));
	  var match$2 = get$1("id", pageObj);
	  if (match$2 === undefined) {
	    fatal(buildFatalMessage("parse", "file could not be found", "", "", ""));
	  }
	  return /* record */[
	          /* commonHeight */unsafeGet$1("lineHeight", commonObj),
	          /* commonBase */unsafeGet$1("base", commonObj),
	          /* scaleW */unsafeGet$1("scaleW", commonObj),
	          /* scaleH */unsafeGet$1("scaleH", commonObj),
	          /* fontDefDictionary */_parseChar(fntStr),
	          /* kerningMap */_parseKerning(fntStr)
	        ];
	}


	/* Log-WonderLog Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function addFont$2(param, record) {
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData *//* record */[
	            /* fntFilePath */param[0],
	            /* bitmapFilePath */param[1]
	          ],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData */record[/* extendData */8]
	        ];
	}

	function load$2(fetchFunc, handleWhenLoadingFunc, record) {
	  var assetData = record[/* assetData */1];
	  var bitmapId = assetData[/* bitmapId */1];
	  var fntId = assetData[/* fntId */0];
	  var match = unsafeGetFontData(record);
	  var bitmapMap = assetData[/* bitmapMap */3];
	  var fntDataMap = assetData[/* fntDataMap */2];
	  var bitmapFilePath = match[/* bitmapFilePath */1];
	  var fntFilePath = match[/* fntFilePath */0];
	  return drain(merge(map$2((function (fntData) {
	                          set(fntId, fntData, fntDataMap);
	                          return /* () */0;
	                        }), map$2((function (fntStr) {
	                              return parse(fntStr, fntFilePath);
	                            }), fromPromise(fetchFunc(fntFilePath).then((function (response) {
	                                        _2(handleWhenLoadingFunc, getContentLength(response), fntFilePath);
	                                        return Promise.resolve(response);
	                                      })).then((function (prim) {
	                                      return prim.text();
	                                    }))))), map$2((function (image) {
	                          set(bitmapId, image, bitmapMap);
	                          return /* () */0;
	                        }), flatMap((function (blob) {
	                              return tap((function (image) {
	                                            return revokeObjectURL(blob);
	                                          }), loadImageByBlobPromise(createObjectURL(blob)));
	                            }), fromPromise(fetchFunc(bitmapFilePath).then((function (response) {
	                                        _2(handleWhenLoadingFunc, getContentLength(response), bitmapFilePath);
	                                        return Promise.resolve(response);
	                                      })).then((function (prim) {
	                                      return prim.blob();
	                                    }))))))).then((function (param) {
	                return Promise.resolve(record);
	              }));
	}


	/* most Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function getFntData$1(record) {
	  var assetData = record[/* assetData */1];
	  return get$1(assetData[/* fntId */0], assetData[/* fntDataMap */2]);
	}

	function setFntData$1(fntData, record) {
	  var assetData = record[/* assetData */1];
	  set(assetData[/* fntId */0], fntData, assetData[/* fntDataMap */2]);
	  return record;
	}

	function getBitmap$1(record) {
	  var assetData = record[/* assetData */1];
	  return get$1(assetData[/* bitmapId */1], assetData[/* bitmapMap */3]);
	}

	function unsafeGetBitmap(record) {
	  return unsafeGet(getBitmap$1(record));
	}

	function setBitmap$1(bitmap, record) {
	  var assetData = record[/* assetData */1];
	  set(assetData[/* bitmapId */1], bitmap, assetData[/* bitmapMap */3]);
	  return record;
	}

	function isLoadAsset(record) {
	  return isSome(getBitmap$1(record));
	}

	function load$3(customTextureSourceDataArr, param, record) {
	  var handleWhenLoadingFunc = param[1];
	  var fetchFunc = param[0];
	  var customImageArr = record[/* assetData */1][/* customImageArr */4];
	  var imguiRecord = /* record */[/* contents */1];
	  return drain(mergeArray(/* array */[fromPromise(load$2(fetchFunc, handleWhenLoadingFunc, record).then((function (record) {
	                                  imguiRecord[0] = record;
	                                  return Promise.resolve(/* () */0);
	                                })))].concat(customTextureSourceDataArr.map((function (param) {
	                              var imageId = param[1];
	                              var imagePath = param[0];
	                              return map$2((function (image) {
	                                            push(/* tuple */[
	                                                  image,
	                                                  imageId,
	                                                  getType(imagePath)
	                                                ], customImageArr);
	                                            return /* () */0;
	                                          }), flatMap((function (blob) {
	                                                return tap((function (image) {
	                                                              return revokeObjectURL(blob);
	                                                            }), loadImageByBlobPromise(createObjectURL(blob)));
	                                              }), fromPromise(fetchFunc(imagePath).then((function (response) {
	                                                          _2(handleWhenLoadingFunc, getContentLength(response), imagePath);
	                                                          return Promise.resolve(response);
	                                                        })).then((function (prim) {
	                                                        return prim.blob();
	                                                      })))));
	                            }))))).then((function (param) {
	                var init = imguiRecord[0];
	                var init$1 = imguiRecord[0][/* assetData */1];
	                return Promise.resolve(/* record */[
	                            /* setting */init[/* setting */0],
	                            /* assetData : record */[
	                              /* fntId */init$1[/* fntId */0],
	                              /* bitmapId */init$1[/* bitmapId */1],
	                              /* fntDataMap */init$1[/* fntDataMap */2],
	                              /* bitmapMap */init$1[/* bitmapMap */3],
	                              /* customImageArr */customImageArr,
	                              /* customTextureMap */init$1[/* customTextureMap */5]
	                            ],
	                            /* fontData */init[/* fontData */2],
	                            /* webglData */init[/* webglData */3],
	                            /* drawData */init[/* drawData */4],
	                            /* imguiFuncData */init[/* imguiFuncData */5],
	                            /* ioData */init[/* ioData */6],
	                            /* layoutData */init[/* layoutData */7],
	                            /* extendData */init[/* extendData */8]
	                          ]);
	              }));
	}

	function createCustomTextures(gl, customImageArr, customTextureMap) {
	  var rgb = gl.RGB;
	  var rgba = gl.RGBA;
	  var target = gl.TEXTURE_2D;
	  return reduceOneParam((function (customTextureMap, data) {
	                var format;
	                switch (data[2]) {
	                  case 0 : 
	                      format = rgb;
	                      break;
	                  case 1 : 
	                      format = rgba;
	                      break;
	                  case 2 : 
	                      format = fatal(buildFatalMessage("createCustomTextures", "unknown image type. type should be jpg or png.", "", "", ""));
	                      break;
	                  
	                }
	                var texture = gl.createTexture();
	                gl.bindTexture(target, texture);
	                gl.texParameteri(target, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
	                gl.texImage2D(target, 0, format, format, gl.UNSIGNED_BYTE, data[0]);
	                return set(data[1], texture, customTextureMap);
	              }), customTextureMap, customImageArr);
	}

	function unsafeGetCustomTexture(id, param) {
	  return unsafeGet$1(id, param[/* assetData */1][/* customTextureMap */5]);
	}

	function getCustomImageArr$1(param) {
	  return param[/* assetData */1][/* customImageArr */4];
	}

	function setCustomImageArr$1(customImageArr, record) {
	  var init = record[/* assetData */1];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData : record */[
	            /* fntId */init[/* fntId */0],
	            /* bitmapId */init[/* bitmapId */1],
	            /* fntDataMap */init[/* fntDataMap */2],
	            /* bitmapMap */init[/* bitmapMap */3],
	            /* customImageArr */customImageArr,
	            /* customTextureMap */init[/* customTextureMap */5]
	          ],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData */record[/* extendData */8]
	        ];
	}


	/* most Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function load$1(customTextureSourceDataArr, handleWhenLoadingFunc, record) {
	  return load$3((customTextureSourceDataArr == null) ? /* array */[] : customTextureSourceDataArr, /* tuple */[
	              fetch$1,
	              handleWhenLoadingFunc
	            ], record);
	}

	function addFont$1(fntFilePath, bitmapFilePath, record) {
	  return addFont$2(/* tuple */[
	              fntFilePath,
	              bitmapFilePath
	            ], record);
	}

	function buildIOData$1(pointUp, pointDown, pointPosition, pointMovementDelta, param) {
	  return buildIOData$2(pointUp, pointDown, pointPosition, pointMovementDelta, /* () */0);
	}


	/* FetchService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	var getFntData$2 = getFntData$1;

	var setFntData$2 = setFntData$1;

	var getBitmap$2 = getBitmap$1;

	var setBitmap$2 = setBitmap$1;

	var getCustomImageArr$2 = getCustomImageArr$1;

	var setCustomImageArr$2 = setCustomImageArr$1;


	/* AssetIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function set$2(key, value, map) {
	  var newMap = copy$4(map);
	  newMap[key] = value;
	  return newMap;
	}

	function deleteVal$2(key, map) {
	  var newMap = copy$4(map);
	  newMap[key] = undefined;
	  return newMap;
	}

	var createEmpty$5 = createEmpty$2;

	var unsafeGet$5 = unsafeGet$2;


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function getAllSkinDataMap(record) {
	  return record[/* extendData */8][/* skinData */1][/* allSkinDataMap */0];
	}

	function clearAllSkins$2(record) {
	  var init = record[/* extendData */8];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData : record */[
	            /* customControlData */init[/* customControlData */0],
	            /* skinData : record */[/* allSkinDataMap */createEmpty$5(/* () */0)]
	          ]
	        ];
	}

	function addSkinData$2(skinName, skinData, record) {
	  var init = record[/* extendData */8];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData : record */[
	            /* customControlData */init[/* customControlData */0],
	            /* skinData : record */[/* allSkinDataMap */set$2(skinName, skinData, record[/* extendData */8][/* skinData */1][/* allSkinDataMap */0])]
	          ]
	        ];
	}

	function removeSkinData$2(skinName, record) {
	  var init = record[/* extendData */8];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData : record */[
	            /* customControlData */init[/* customControlData */0],
	            /* skinData : record */[/* allSkinDataMap */deleteVal$2(skinName, record[/* extendData */8][/* skinData */1][/* allSkinDataMap */0])]
	          ]
	        ];
	}

	function unsafeGetSkinData(skinName, record) {
	  return unsafeGet$5(skinName, record[/* extendData */8][/* skinData */1][/* allSkinDataMap */0]);
	}

	function createSkinData$2(allCustomStyleData) {
	  return /* record */[/* allCustomStyleData */allCustomStyleData];
	}

	function createAllCustomStyleData$2(param) {
	  return createEmpty$5(/* () */0);
	}

	function createSingleCustomStyleData$2(param) {
	  return createEmpty$5(/* () */0);
	}

	var addCustomStyleData$2 = set$2;

	var removeCustomStyleData$2 = deleteVal$2;

	var addSingleCustomStyleData$2 = set$2;

	var removeSingleCustomStyleData$2 = deleteVal$2;

	function unsafeGetSingleCustomStyleDataMap(singleCustomStyleName, skinData) {
	  return unsafeGet$5(singleCustomStyleName, skinData[/* allCustomStyleData */0]);
	}

	var unsafeGetCustomStyleData = unsafeGet$5;


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function getDefaultSkinName(param) {
	  return "Wonder_Default_Skin";
	}

	function getDefaultCustomStyleName(param) {
	  return "Wonder_Default_CustomStyle";
	}

	function _createDefaultSkinData(param) {
	  var allCustomStyleData = createAllCustomStyleData$2(/* () */0);
	  var singleCustomStyleData = createSingleCustomStyleData$2(/* () */0);
	  return createSkinData$2(addSingleCustomStyleData$2("Wonder_Default_CustomStyle", singleCustomStyleData, allCustomStyleData));
	}

	function addDefaultSkinData$2(record) {
	  var init = record[/* extendData */8];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData : record */[
	            /* customControlData */init[/* customControlData */0],
	            /* skinData : record */[/* allSkinDataMap */set$2("Wonder_Default_Skin", _createDefaultSkinData(/* () */0), getAllSkinDataMap(record))]
	          ]
	        ];
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function registerCustomControl$2(name, customControlFunc, record) {
	  var extendData = record[/* extendData */8];
	  var init = extendData[/* customControlData */0];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData : record */[
	            /* customControlData : record */[
	              /* apiJsObj */init[/* apiJsObj */0],
	              /* funcMap */set$2(name, (function (customControlFuncInputData, customControlFunctionShowData, imguiRecord) {
	                      return customControlFunc(customControlFuncInputData, customControlFunctionShowData, imguiRecord[/* extendData */8][/* customControlData */0][/* apiJsObj */0], imguiRecord);
	                    }), record[/* extendData */8][/* customControlData */0][/* funcMap */1])
	            ],
	            /* skinData */extendData[/* skinData */1]
	          ]
	        ];
	}

	function unsafeGetCustomControl(name, record) {
	  return unsafeGet$5(name, record[/* extendData */8][/* customControlData */0][/* funcMap */1]);
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	var registerCustomControl$1 = registerCustomControl$2;

	var createAllCustomStyleData$1 = createAllCustomStyleData$2;

	var createSingleCustomStyleData$1 = createSingleCustomStyleData$2;

	var addCustomStyleData$1 = addCustomStyleData$2;

	var removeCustomStyleData$1 = removeCustomStyleData$2;

	var addSingleCustomStyleData$1 = addSingleCustomStyleData$2;

	var removeSingleCustomStyleData$1 = removeSingleCustomStyleData$2;

	var addSkinData$1 = addSkinData$2;

	var removeSkinData$1 = removeSkinData$2;

	var createSkinData$1 = createSkinData$2;

	var clearAllSkins$1 = clearAllSkins$2;

	var addDefaultSkinData$1 = addDefaultSkinData$2;


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE


	var vs = "\nprecision mediump float;\n\nattribute vec2 a_position;\nattribute vec3 a_color;\nattribute vec2 a_texCoord;\n\nuniform mat4 u_projectionMat;\n\nvarying vec3 v_color;\nvarying vec2 v_texCoord;\n\nvoid main() {\n  gl_Position = u_projectionMat * vec4(a_position, 0, 1);\n  v_color = a_color;\n  v_texCoord = a_texCoord;\n}\n    ";

	var fs = "\nprecision mediump float;\n\nvarying vec3 v_color;\nvarying vec2 v_texCoord;\n\nuniform sampler2D u_sampler2D;\n\nvoid main() {\n  vec4 sample = texture2D(u_sampler2D, v_texCoord);\n\n  //gl_FragColor = vec4(v_color.xyz * sample.xyz, sample.x * vColor.a );\n  gl_FragColor = vec4(v_color * sample.xyz, sample.w);\n}\n    ";


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE


	function createIdentityMatrix4(param) {
	  return new Float32Array(/* array */[
	              1,
	              0,
	              0,
	              0,
	              0,
	              1,
	              0,
	              0,
	              0,
	              0,
	              1,
	              0,
	              0,
	              0,
	              0,
	              1
	            ]);
	}

	function ortho(left, right, bottom, top, near, far, resultFloat32Arr) {
	  var lr = 1 / (left - right);
	  var bt = 1 / (bottom - top);
	  var nf = 1 / (near - far);
	  resultFloat32Arr[0] = -2 * lr;
	  resultFloat32Arr[1] = 0;
	  resultFloat32Arr[2] = 0;
	  resultFloat32Arr[3] = 0;
	  resultFloat32Arr[4] = 0;
	  resultFloat32Arr[5] = -2 * bt;
	  resultFloat32Arr[6] = 0;
	  resultFloat32Arr[7] = 0;
	  resultFloat32Arr[8] = 0;
	  resultFloat32Arr[9] = 0;
	  resultFloat32Arr[10] = 2 * nf;
	  resultFloat32Arr[11] = 0;
	  resultFloat32Arr[12] = (left + right) * lr;
	  resultFloat32Arr[13] = (top + bottom) * bt;
	  resultFloat32Arr[14] = (far + near) * nf;
	  resultFloat32Arr[15] = 1;
	  return resultFloat32Arr;
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _compileShader(gl, glslSource, shader) {
	  gl.shaderSource(shader, glslSource);
	  gl.compileShader(shader);
	  debugWithFunc((function (param) {
	          var match = gl.getShaderParameter(shader, gl.COMPILE_STATUS) === false;
	          if (match) {
	            var message = gl.getShaderInfoLog(shader);
	            var partial_arg = "" + (String(message) + "");
	            debug((function (param) {
	                    return buildDebugMessage("shader info log", partial_arg, param);
	                  }), true);
	            var partial_arg$1 = "" + (String(glslSource) + "");
	            return debug((function (param) {
	                          return buildDebugMessage("glsl source", partial_arg$1, param);
	                        }), true);
	          } else {
	            return /* () */0;
	          }
	        }), true);
	  return shader;
	}

	function _linkProgram(program, gl) {
	  gl.linkProgram(program);
	  debugWithFunc((function (param) {
	          var match = gl.getProgramParameter(program, gl.LINK_STATUS) === false;
	          if (match) {
	            var message = gl.getProgramInfoLog(program);
	            return fatal(buildFatalMessage("link program error", "" + (String(message) + ""), "", "", ""));
	          } else {
	            return /* () */0;
	          }
	        }), true);
	  return /* () */0;
	}

	function initShader(vsSource, fsSource, gl, program) {
	  var vs = _compileShader(gl, vsSource, gl.createShader(gl.VERTEX_SHADER));
	  var fs = _compileShader(gl, fsSource, gl.createShader(gl.FRAGMENT_SHADER));
	  gl.attachShader(program, vs);
	  gl.attachShader(program, fs);
	  gl.bindAttribLocation(program, 0, "a_position");
	  _linkProgram(program, gl);
	  gl.deleteShader(vs);
	  gl.deleteShader(fs);
	  return program;
	}


	/* Log-WonderLog Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function addPoints(points, pointArr) {
	  return reduceOneParam((function (arr, point) {
	                return push(point, arr);
	              }), pointArr, points);
	}

	function concatArrays (arrays){
	  return [].concat.apply([], arrays);  
	  }

	function getBaseIndex(verticeArr) {
	  return verticeArr.length / 2 | 0;
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _bufferArrayBufferData(param, gl) {
	  var $$location = param[2];
	  gl.bindBuffer(gl.ARRAY_BUFFER, param[0]);
	  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(param[1]), gl.DYNAMIC_DRAW);
	  gl.enableVertexAttribArray($$location);
	  gl.vertexAttribPointer($$location, param[3], gl.FLOAT, false, 0, 0);
	  gl.bindBuffer(gl.ARRAY_BUFFER, null);
	  return gl;
	}

	function _bufferElementArrayBufferData(buffer, pointArr, gl) {
	  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, buffer);
	  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(pointArr), gl.DYNAMIC_DRAW);
	  return gl;
	}

	function bufferAllData(gl, groupedDrawDataArr, record) {
	  var match = unsafeGetWebglData(record);
	  var match$1 = reduceOneParam((function (param, param$1) {
	          var indexArr = param$1[/* indexArr */5];
	          var totalIndexArr = param[5];
	          var totalTexCoordArr = param[4];
	          var totalColorArr = param[3];
	          var totalVerticeArr = param[2];
	          var countOffset = param[1];
	          var drawElementsDataArr = param[0];
	          var count = indexArr.length;
	          if (count !== 0) {
	            var newCountOffset = countOffset + (count << 1) | 0;
	            return /* tuple */[
	                    push(/* record */[
	                          /* drawType */param$1[/* drawType */0],
	                          /* customTexture */param$1[/* customTexture */1],
	                          /* count */count,
	                          /* countOffset */countOffset
	                        ], drawElementsDataArr),
	                    newCountOffset,
	                    totalVerticeArr.concat(param$1[/* verticeArr */2]),
	                    totalColorArr.concat(param$1[/* colorArr */3]),
	                    totalTexCoordArr.concat(param$1[/* texCoordArr */4]),
	                    totalIndexArr.concat(indexArr)
	                  ];
	          } else {
	            return /* tuple */[
	                    drawElementsDataArr,
	                    0,
	                    totalVerticeArr,
	                    totalColorArr,
	                    totalTexCoordArr,
	                    totalIndexArr
	                  ];
	          }
	        }), /* tuple */[
	        /* array */[],
	        0,
	        /* array */[],
	        /* array */[],
	        /* array */[],
	        /* array */[]
	      ], groupedDrawDataArr);
	  _bufferElementArrayBufferData(match[/* indexBuffer */4], match$1[5], _bufferArrayBufferData(/* tuple */[
	            match[/* texCoordBuffer */3],
	            match$1[4],
	            match[/* aTexCoordLocation */8],
	            2
	          ], _bufferArrayBufferData(/* tuple */[
	                match[/* colorBuffer */2],
	                match$1[3],
	                match[/* aColorLocation */7],
	                3
	              ], _bufferArrayBufferData(/* tuple */[
	                    match[/* positionBuffer */1],
	                    match$1[2],
	                    match[/* aPositonLocation */6],
	                    2
	                  ], gl))));
	  return /* tuple */[
	          record,
	          match$1[0]
	        ];
	}


	/* RecordIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function beginGroup$1(position, record) {
	  var groupData = record[/* layoutData */7][/* groupData */0];
	  groupData[/* positionArr */0][groupData[/* index */1]] = position;
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData : record */[/* groupData : record */[
	              /* positionArr */groupData[/* positionArr */0],
	              /* index */groupData[/* index */1] + 1 | 0
	            ]],
	          /* extendData */record[/* extendData */8]
	        ];
	}

	function endGroup$1(record) {
	  var groupData = record[/* layoutData */7][/* groupData */0];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData : record */[/* groupData : record */[
	              /* positionArr */groupData[/* positionArr */0],
	              /* index */groupData[/* index */1] - 1 | 0
	            ]],
	          /* extendData */record[/* extendData */8]
	        ];
	}

	function _getGroupPosition(record) {
	  var groupData = record[/* layoutData */7][/* groupData */0];
	  var match = groupData[/* index */1] <= 0;
	  if (match) {
	    return undefined;
	  } else {
	    var match$1 = reduceOneParam((function (param, index) {
	            var match = param[1];
	            var positionArr = param[0];
	            var match$1 = positionArr[index];
	            return /* tuple */[
	                    positionArr,
	                    /* tuple */[
	                      match[0] + match$1[0] | 0,
	                      match[1] + match$1[1] | 0
	                    ]
	                  ];
	          }), /* tuple */[
	          groupData[/* positionArr */0],
	          /* tuple */[
	            0,
	            0
	          ]
	        ], range(0, groupData[/* index */1] - 1 | 0));
	    return match$1[1];
	  }
	}

	function computeRectBasedOnTopLeftOfView(rect, record) {
	  var match = _getGroupPosition(record);
	  if (match !== undefined) {
	    var match$1 = match;
	    return /* tuple */[
	            rect[0] + match$1[0] | 0,
	            rect[1] + match$1[1] | 0,
	            rect[2],
	            rect[3]
	          ];
	  } else {
	    return rect;
	  }
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function draw(param, color, record) {
	  var height = param[3];
	  var width = param[2];
	  var y = param[1];
	  var x = param[0];
	  var match = getSetting$1(record);
	  var fontTexUvForWhite = match[/* fontTexUvForWhite */1];
	  var match$1 = getFontTextureDrawData(record);
	  var verticeArr = match$1[/* verticeArr */2];
	  var baseIndex = getBaseIndex(verticeArr);
	  var init = record[/* drawData */4];
	  var init$1 = record[/* drawData */4][/* fontTextureDrawData */0];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData : record */[
	            /* fontTextureDrawData : record */[
	              /* drawType */init$1[/* drawType */0],
	              /* customTexture */init$1[/* customTexture */1],
	              /* verticeArr */addPoints(/* array */[
	                    x,
	                    y,
	                    x,
	                    y + height,
	                    x + width,
	                    y,
	                    x + width,
	                    y + height
	                  ], verticeArr),
	              /* colorArr */addPoints(concatArrays(/* array */[
	                        color,
	                        color,
	                        color,
	                        color
	                      ]), match$1[/* colorArr */3]),
	              /* texCoordArr */addPoints(concatArrays(/* array */[
	                        fontTexUvForWhite,
	                        fontTexUvForWhite,
	                        fontTexUvForWhite,
	                        fontTexUvForWhite
	                      ]), match$1[/* texCoordArr */4]),
	              /* indexArr */addPoints(/* array */[
	                    baseIndex,
	                    baseIndex + 1 | 0,
	                    baseIndex + 2 | 0,
	                    baseIndex + 3 | 0,
	                    baseIndex + 2 | 0,
	                    baseIndex + 1 | 0
	                  ], match$1[/* indexArr */5])
	            ],
	            /* customTextureDrawDataMap */init[/* customTextureDrawDataMap */1]
	          ],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData */record[/* extendData */8]
	        ];
	}


	/* RecordIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function getKerning(fntData, left, right) {
	  var match = get$4(buildKerningHashMapKey(left, right), fntData[/* kerningMap */5]);
	  if (match !== undefined) {
	    return match;
	  } else {
	    return 0;
	  }
	}


	/* ParseFntIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _getGlyphById(fntData, id) {
	  var dict = fntData[/* fontDefDictionary */4];
	  return get$4(id, dict);
	}

	function getGlyph(param, fntData, id) {
	  _getGlyphById(fntData, id);
	  var match = _getGlyphById(fntData, id);
	  if (match !== undefined) {
	    return match;
	  } else {
	    var match$1 = id === "\t".charCodeAt(0);
	    if (match$1) {
	      return param[1];
	    } else {
	      var match$2 = id === " ".charCodeAt(0);
	      if (match$2) {
	        return param[0];
	      } else {
	        return undefined;
	      }
	    }
	  }
	}

	function _getMGlyph(fntData) {
	  var m_widthArr = /* array */[
	    "m",
	    "w"
	  ];
	  return reduceOneParam((function (glyph, m_width) {
	                if (glyph !== undefined) {
	                  return glyph;
	                } else {
	                  return _getGlyphById(fntData, m_width.charCodeAt(0));
	                }
	              }), undefined, m_widthArr);
	}

	function _getFirstGlyph(fntData) {
	  return getValidValues$2(fntData[/* fontDefDictionary */4])[0];
	}

	function setupSpaceGlyphs(fntData, tabSize) {
	  var match = _getGlyphById(fntData, " ".charCodeAt(0));
	  var space;
	  if (match !== undefined) {
	    space = match;
	  } else {
	    var match$1 = _getMGlyph(fntData);
	    space = match$1 !== undefined ? match$1 : _getFirstGlyph(fntData);
	  }
	  return /* tuple */[
	          space,
	          /* record */[
	            /* id */"\t".charCodeAt(0),
	            /* rect : tuple */[
	              0,
	              0,
	              0,
	              0
	            ],
	            /* xOffset */0,
	            /* yOffset */0,
	            /* xAdvance */imul(tabSize, space[/* xAdvance */4])
	          ]
	        ];
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function hasFontDefDictionaryData(param) {
	  return length$2(param[/* fontDefDictionary */4]) > 0;
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE



	function _computeMetrics(fntData, text, letterSpacing, start, end_, width, hasFontDefDictionaryDataFunc, getGlyphFunc, getKerningFunc) {
	  var match = !_1(hasFontDefDictionaryDataFunc, fntData);
	  if (match) {
	    return /* tuple */[
	            start,
	            start,
	            0
	          ];
	  } else {
	    var curPen = 0;
	    var curWidth = 0;
	    var count = 0;
	    var lastGlyph = undefined;
	    var $$break = false;
	    for(var i = start ,i_finish = Math.min(text.length, end_) - 1 | 0; i <= i_finish; ++i){
	      var match$1 = $$break;
	      if (!match$1) {
	        var id = text.charCodeAt(i);
	        var glyph = _2(getGlyphFunc, fntData, id);
	        if (glyph !== undefined) {
	          var glyph$1 = glyph;
	          var match$2 = glyph$1[/* rect */1];
	          var match$3 = lastGlyph;
	          var kern = match$3 !== undefined ? _3(getKerningFunc, fntData, match$3[/* id */0], glyph$1[/* id */0]) : 0;
	          curPen = curPen + kern | 0;
	          var nextPen = (curPen + glyph$1[/* xAdvance */4] | 0) + letterSpacing | 0;
	          var nextWidth = curPen + match$2[2] | 0;
	          var match$4 = nextWidth > width || nextPen > width;
	          if (match$4) {
	            var match$5 = count === 0;
	            if (match$5) {
	              count = 1;
	              curWidth = nextWidth;
	            }
	            $$break = true;
	          }
	          curPen = nextPen;
	          curWidth = nextWidth;
	          lastGlyph = glyph$1;
	        }
	        count = count + 1 | 0;
	      }
	      
	    }
	    var match$6 = lastGlyph;
	    if (match$6 !== undefined) {
	      curWidth = curWidth + match$6[/* xOffset */2] | 0;
	    }
	    return /* tuple */[
	            start,
	            start + count | 0,
	            curWidth
	          ];
	  }
	}

	function _findNewLineIndex(text, $$char, start, end_) {
	  var idx = text.indexOf($$char, start);
	  var match = idx === -1 || idx > end_;
	  if (match) {
	    return end_;
	  } else {
	    return idx;
	  }
	}

	function _isWhitespace($$char) {
	  return (/\s/).test($$char);
	}

	function _greedy (fntData,text,letterSpacing,start,end_,width,hasFontDefDictionaryDataFunc,getGlyphFunc,getKerningFunc){
	             /* A greedy word wrapper based on LibGDX algorithm
	            https://github.com/libgdx/libgdx/blob/master/gdx/src/com/badlogic/gdx/graphics/g2d/BitmapFontCache.java */
	    const NEWLINE_CHAR = '\n';

	            var lines = [],
	                textWidth = width;

	            while (start < end_ && start < text.length) {
	                /* get next newline position */
	                let newLine = _findNewLineIndex(text, NEWLINE_CHAR, start, end_);

	                /* eat whitespace at start of line */
	                while (start < newLine) {
	                    if (!_isWhitespace( text.charAt(start) )){
	                        break;
	                    }

	                    start++;
	                }

	                /* determine visible # of glyphs for the available width */
	                let measured = _computeMetrics(fntData, text, letterSpacing, start, newLine, textWidth, hasFontDefDictionaryDataFunc, getGlyphFunc, getKerningFunc),
	                    lineEnd = start + (measured[1]-measured[0]),
	                    nextStart = lineEnd + NEWLINE_CHAR.length;


	                /* if we had to cut the line before the next newline... */
	                if (lineEnd < newLine) {
	                    /* find char to break on */
	                    while (lineEnd > start) {
	                        if (_isWhitespace(text.charAt(lineEnd))){
	                            break;
	                        }

	                        lineEnd--;
	                    }

	                    if (lineEnd === start) {
	                        if (nextStart > start + NEWLINE_CHAR.length){
	                            nextStart--;
	                        }

	 /* If no characters to break, show all. */
	                        lineEnd = nextStart ;
	                    }
	                    else {
	                        nextStart = lineEnd;

	                        /* eat whitespace at end_ of line */
	                        while (lineEnd > start) {
	                            if (!_isWhitespace(text.charAt(lineEnd - NEWLINE_CHAR.length))){
	                                break;
	                            }

	                            lineEnd--;
	                        }
	                    }
	                }

	                if (lineEnd >= start) {
	                    lines.push(_computeMetrics(fntData, text, letterSpacing, start, lineEnd, textWidth, hasFontDefDictionaryDataFunc, getGlyphFunc, getKerningFunc));
	                }

	                start = nextStart;
	            }
	            return lines


	            }

	function getLines(fntData, text, param, fallbackGlyphTuple) {
	  return _greedy(fntData, text, param[0], param[2], param[3], param[1], hasFontDefDictionaryData, (function (param, param$1) {
	                return getGlyph(fallbackGlyphTuple, param, param$1);
	              }), getKerning);
	}


	/* BitmapFontParserIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _computeYForCenterYAlignment(totalHeight, lineHeight, lines) {
	  var linesHeight = imul(lines.length, lineHeight);
	  return (totalHeight - linesHeight | 0) / 2 | 0;
	}

	function getLayoutData(text, param, fntData, record) {
	  var align = param[4];
	  var letterSpacing = param[3];
	  var width = param[0];
	  var fallbackGlyphTuple = setupSpaceGlyphs(fntData, param[2]);
	  var lines = getLines(fntData, text, /* tuple */[
	        letterSpacing,
	        width,
	        0,
	        text.length
	      ], fallbackGlyphTuple);
	  var lineHeight = fntData[/* commonHeight */0];
	  var maxLineWidth = reduceOneParam((function (width$1, param) {
	          return Math.max(width$1, param[2], width);
	        }), 0, lines);
	  return reduceOneParami((function (param, param$1, lineIndex) {
	                  var lineWidth = param$1[2];
	                  var layoutDataArr = param[0];
	                  var lastGlyph = undefined;
	                  var x = param[1];
	                  var y = param[2];
	                  for(var i = param$1[0] ,i_finish = param$1[1] - 1 | 0; i <= i_finish; ++i){
	                    var id = text.charCodeAt(i);
	                    var match = getGlyph(fallbackGlyphTuple, fntData, id);
	                    if (match !== undefined) {
	                      var glyph = match;
	                      var match$1 = lastGlyph;
	                      var tx;
	                      if (match$1 !== undefined) {
	                        x = x + getKerning(fntData, match$1[/* id */0], glyph[/* id */0]) | 0;
	                        tx = x;
	                      } else {
	                        tx = x;
	                      }
	                      var tx$1;
	                      switch (align) {
	                        case 0 : 
	                            tx$1 = tx;
	                            break;
	                        case 1 : 
	                            tx$1 = tx + ((maxLineWidth - lineWidth | 0) / 2 | 0) | 0;
	                            break;
	                        case 2 : 
	                            tx$1 = tx + (maxLineWidth - lineWidth | 0) | 0;
	                            break;
	                        
	                      }
	                      push(/* record */[
	                            /* position : tuple */[
	                              tx$1 + glyph[/* xOffset */2] | 0,
	                              y + glyph[/* yOffset */3] | 0
	                            ],
	                            /* data */glyph,
	                            /* index */i,
	                            /* line */lineIndex
	                          ], layoutDataArr);
	                      x = (x + glyph[/* xAdvance */4] | 0) + letterSpacing | 0;
	                      lastGlyph = glyph;
	                    }
	                    
	                  }
	                  return /* tuple */[
	                          layoutDataArr,
	                          0,
	                          y + lineHeight | 0,
	                          lastGlyph
	                        ];
	                }), /* tuple */[
	                /* array */[],
	                0,
	                _computeYForCenterYAlignment(param[1], lineHeight, lines),
	                undefined
	              ], lines)[0];
	}


	/* BitmapFontParserIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _generateVertices(posX, posY, param, verticeArr) {
	  var position = param[/* position */0];
	  var match = param[/* data */1][/* rect */1];
	  var charHeightInImage = match[3];
	  var charWidthInImage = match[2];
	  var leftUpX = position[0] + posX;
	  var leftUpY = position[1] + posY;
	  return addPoints(/* array */[
	              leftUpX,
	              leftUpY,
	              leftUpX,
	              leftUpY + charHeightInImage,
	              leftUpX + charWidthInImage,
	              leftUpY,
	              leftUpX + charWidthInImage,
	              leftUpY + charHeightInImage
	            ], verticeArr);
	}

	function _generateTexCoords(param, textureWidth, textureHeight, texCoordArr) {
	  var match = param[/* data */1][/* rect */1];
	  var charYInImage = match[1];
	  var charXInImage = match[0];
	  var s0 = charXInImage / textureWidth;
	  var t0 = charYInImage / textureHeight;
	  var s1 = (charXInImage + match[2] | 0) / textureWidth;
	  var t1 = (charYInImage + match[3] | 0) / textureHeight;
	  return addPoints(/* array */[
	              s0,
	              t0,
	              s0,
	              t1,
	              s1,
	              t0,
	              s1,
	              t1
	            ], texCoordArr);
	}

	function _generateIndices(baseIndex, indexArr) {
	  return addPoints(/* array */[
	              baseIndex,
	              baseIndex + 1 | 0,
	              baseIndex + 2 | 0,
	              baseIndex + 3 | 0,
	              baseIndex + 2 | 0,
	              baseIndex + 1 | 0
	            ], indexArr);
	}

	function draw$1(param, str, align, record) {
	  var y = param[1];
	  var x = param[0];
	  var match = getSetting$1(record);
	  var textColor = match[/* textColor */0];
	  var textColorArrPerPoint = concatArrays(/* array */[
	        textColor,
	        textColor,
	        textColor,
	        textColor
	      ]);
	  var match$1 = getFntData$1(record);
	  if (match$1 !== undefined) {
	    var fntData = match$1;
	    var layoutDataArr = getLayoutData(str, /* tuple */[
	          param[2],
	          param[3],
	          4,
	          0,
	          align
	        ], fntData, record);
	    var match$2 = getFontTextureDrawData(record);
	    var match$3 = reduceOneParam((function (param, layoutData) {
	            var verticeArr = param[0];
	            var baseIndex = getBaseIndex(verticeArr);
	            return /* tuple */[
	                    _generateVertices(x, y, layoutData, verticeArr),
	                    addPoints(textColorArrPerPoint, param[1]),
	                    _generateTexCoords(layoutData, fntData[/* scaleW */2], fntData[/* scaleH */3], param[2]),
	                    _generateIndices(baseIndex, param[3])
	                  ];
	          }), /* tuple */[
	          match$2[/* verticeArr */2],
	          match$2[/* colorArr */3],
	          match$2[/* texCoordArr */4],
	          match$2[/* indexArr */5]
	        ], layoutDataArr);
	    var init = record[/* drawData */4];
	    var init$1 = record[/* drawData */4][/* fontTextureDrawData */0];
	    return /* record */[
	            /* setting */record[/* setting */0],
	            /* assetData */record[/* assetData */1],
	            /* fontData */record[/* fontData */2],
	            /* webglData */record[/* webglData */3],
	            /* drawData : record */[
	              /* fontTextureDrawData : record */[
	                /* drawType */init$1[/* drawType */0],
	                /* customTexture */init$1[/* customTexture */1],
	                /* verticeArr */match$3[0],
	                /* colorArr */match$3[1],
	                /* texCoordArr */match$3[2],
	                /* indexArr */match$3[3]
	              ],
	              /* customTextureDrawDataMap */init[/* customTextureDrawDataMap */1]
	            ],
	            /* imguiFuncData */record[/* imguiFuncData */5],
	            /* ioData */record[/* ioData */6],
	            /* layoutData */record[/* layoutData */7],
	            /* extendData */record[/* extendData */8]
	          ];
	  } else {
	    return fatal(buildFatalMessage("getLayoutData", "impossible to create font: not find fnt file", "", "", ""));
	  }
	}


	/* Log-WonderLog Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function parseShowData(showData) {
	  if (showData == null) {
	    return /* tuple */[
	            getDefaultSkinName(/* () */0),
	            getDefaultCustomStyleName(/* () */0)
	          ];
	  } else {
	    return showData;
	  }
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function createAPIJsObj(param) {
	  return {
	          getIOData: getIOData,
	          getPointUp: (function (ioData) {
	              return ioData[/* pointUp */0];
	            }),
	          getPointDown: (function (ioData) {
	              return ioData[/* pointDown */1];
	            }),
	          getPointPosition: (function (ioData) {
	              return ioData[/* pointPosition */2];
	            }),
	          isClick: isClick,
	          drawBox: draw,
	          drawText: draw$1,
	          parseShowData: parseShowData,
	          unsafeGetSkinData: unsafeGetSkinData,
	          unsafeGetSingleCustomStyleDataMap: unsafeGetSingleCustomStyleDataMap,
	          unsafeGetCustomStyleData: unsafeGetCustomStyleData
	        };
	}


	/* IOIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE


	function convertIntRectToFloatRect(param) {
	  return /* tuple */[
	          param[0],
	          param[1],
	          param[2],
	          param[3]
	        ];
	}


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _getOrCreateCustomTextureDrawData(id, record) {
	  var match = get$1(id, getCustomTextureDrawDataMap(record));
	  if (match !== undefined) {
	    return match;
	  } else {
	    return /* record */[
	            /* drawType : CustomTexture */1,
	            /* customTexture */some$1(unsafeGetCustomTexture(id, record)),
	            /* verticeArr : array */[],
	            /* colorArr : array */[],
	            /* texCoordArr : array */[],
	            /* indexArr : array */[]
	          ];
	  }
	}

	function draw$2(param, param$1, id, record) {
	  var t1 = param$1[3];
	  var s1 = param$1[2];
	  var t0 = param$1[1];
	  var s0 = param$1[0];
	  var height = param[3];
	  var width = param[2];
	  var y = param[1];
	  var x = param[0];
	  var drawData = _getOrCreateCustomTextureDrawData(id, record);
	  var verticeArr = drawData[/* verticeArr */2];
	  var baseIndex = getBaseIndex(verticeArr);
	  var drawData_000 = /* drawType */drawData[/* drawType */0];
	  var drawData_001 = /* customTexture */drawData[/* customTexture */1];
	  var drawData_002 = /* verticeArr */addPoints(/* array */[
	        x,
	        y,
	        x,
	        y + height,
	        x + width,
	        y,
	        x + width,
	        y + height
	      ], verticeArr);
	  var drawData_003 = /* colorArr */addPoints(/* array */[
	        1,
	        1,
	        1,
	        1,
	        1,
	        1,
	        1,
	        1,
	        1,
	        1,
	        1,
	        1
	      ], drawData[/* colorArr */3]);
	  var drawData_004 = /* texCoordArr */addPoints(/* array */[
	        s0,
	        t0,
	        s0,
	        t1,
	        s1,
	        t0,
	        s1,
	        t1
	      ], drawData[/* texCoordArr */4]);
	  var drawData_005 = /* indexArr */addPoints(/* array */[
	        baseIndex,
	        baseIndex + 1 | 0,
	        baseIndex + 2 | 0,
	        baseIndex + 3 | 0,
	        baseIndex + 2 | 0,
	        baseIndex + 1 | 0
	      ], drawData[/* indexArr */5]);
	  var drawData$1 = /* record */[
	    drawData_000,
	    drawData_001,
	    drawData_002,
	    drawData_003,
	    drawData_004,
	    drawData_005
	  ];
	  var init = record[/* drawData */4];
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData : record */[
	            /* fontTextureDrawData */init[/* fontTextureDrawData */0],
	            /* customTextureDrawDataMap */set(id, drawData$1, record[/* drawData */4][/* customTextureDrawDataMap */1])
	          ],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData */record[/* extendData */8]
	        ];
	}


	/* AssetIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function label$1(rect, str, align, record) {
	  return draw$1(convertIntRectToFloatRect(computeRectBasedOnTopLeftOfView(rect, record)), str, align, record);
	}

	function image$1(rect, uv, id, record) {
	  return draw$2(convertIntRectToFloatRect(computeRectBasedOnTopLeftOfView(rect, record)), uv, id, record);
	}

	function box$1(rect, color, record) {
	  return draw(convertIntRectToFloatRect(computeRectBasedOnTopLeftOfView(rect, record)), color, record);
	}


	/* DrawBoxIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _createArrayBuffer(gl) {
	  var buffer = gl.createBuffer();
	  gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
	  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(/* array */[]), gl.DYNAMIC_DRAW);
	  return buffer;
	}

	function _createElementArrayBuffer(gl) {
	  var buffer = gl.createBuffer();
	  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, buffer);
	  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(/* array */[]), gl.DYNAMIC_DRAW);
	  return buffer;
	}

	function _createFontTexture(gl, source) {
	  var texture = gl.createTexture();
	  var target = gl.TEXTURE_2D;
	  gl.bindTexture(target, texture);
	  gl.texParameteri(target, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
	  gl.texParameteri(target, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
	  gl.texParameteri(target, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	  gl.texParameteri(target, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	  var format = gl.RGBA;
	  gl.texImage2D(target, 0, format, format, gl.UNSIGNED_BYTE, source);
	  return texture;
	}

	function _buildOrthoProjectionMat4TypeArr(param) {
	  return ortho(0, param[0], param[1], 0, -1, 1, createIdentityMatrix4(/* () */0));
	}

	function _sendUniformProjectionMatData(gl, program, canvasSize) {
	  gl.uniformMatrix4fv(gl.getUniformLocation(program, "u_projectionMat"), false, _buildOrthoProjectionMat4TypeArr(canvasSize));
	  return /* () */0;
	}

	function _getProgram(record) {
	  return unsafeGet(record[/* webglData */3])[/* program */0];
	}

	function _sendUniformData(gl, program, canvasSize) {
	  gl.useProgram(program);
	  _sendUniformProjectionMatData(gl, program, canvasSize);
	  gl.uniform1i(gl.getUniformLocation(program, "u_sampler2D"), 0);
	  return /* () */0;
	}

	function sendUniformProjectionMatData$2(gl, canvasSize, record) {
	  var program = _getProgram(record);
	  gl.useProgram(program);
	  gl.uniformMatrix4fv(gl.getUniformLocation(program, "u_projectionMat"), false, _buildOrthoProjectionMat4TypeArr(canvasSize));
	  return record;
	}

	function init$2(gl, canvasSize, record) {
	  var match = !isLoadAsset(record);
	  if (match) {
	    return record;
	  } else {
	    var program = initShader(vs, fs, gl, gl.createProgram());
	    var positionBuffer = _createArrayBuffer(gl);
	    var colorBuffer = _createArrayBuffer(gl);
	    var texCoordBuffer = _createArrayBuffer(gl);
	    var indexBuffer = _createElementArrayBuffer(gl);
	    var fontTexture = _createFontTexture(gl, unsafeGetBitmap(record));
	    _sendUniformData(gl, program, canvasSize);
	    var match$1 = record[/* assetData */1];
	    var init$1 = record[/* assetData */1];
	    return /* record */[
	            /* setting */record[/* setting */0],
	            /* assetData : record */[
	              /* fntId */init$1[/* fntId */0],
	              /* bitmapId */init$1[/* bitmapId */1],
	              /* fntDataMap */init$1[/* fntDataMap */2],
	              /* bitmapMap */init$1[/* bitmapMap */3],
	              /* customImageArr */init$1[/* customImageArr */4],
	              /* customTextureMap */createCustomTextures(gl, match$1[/* customImageArr */4], match$1[/* customTextureMap */5])
	            ],
	            /* fontData */record[/* fontData */2],
	            /* webglData *//* record */[
	              /* program */program,
	              /* positionBuffer */positionBuffer,
	              /* colorBuffer */colorBuffer,
	              /* texCoordBuffer */texCoordBuffer,
	              /* indexBuffer */indexBuffer,
	              /* fontTexture */fontTexture,
	              /* aPositonLocation */gl.getAttribLocation(program, "a_position"),
	              /* aColorLocation */gl.getAttribLocation(program, "a_color"),
	              /* aTexCoordLocation */gl.getAttribLocation(program, "a_texCoord"),
	              /* lastWebglData */undefined
	            ],
	            /* drawData */record[/* drawData */4],
	            /* imguiFuncData */record[/* imguiFuncData */5],
	            /* ioData */record[/* ioData */6],
	            /* layoutData */record[/* layoutData */7],
	            /* extendData */record[/* extendData */8]
	          ];
	  }
	}

	function _createEmptyDrawData(param) {
	  return /* record */[
	          /* fontTextureDrawData : record */[
	            /* drawType : FontTexture */0,
	            /* customTexture */undefined,
	            /* verticeArr : array */[],
	            /* colorArr : array */[],
	            /* texCoordArr : array */[],
	            /* indexArr : array */[]
	          ],
	          /* customTextureDrawDataMap */createEmpty$1(/* () */0)
	        ];
	}

	function _prepare(ioData, param, data) {
	  var record = _1(param[0], data);
	  return _2(param[1], /* record */[
	              /* setting */record[/* setting */0],
	              /* assetData */record[/* assetData */1],
	              /* fontData */record[/* fontData */2],
	              /* webglData */record[/* webglData */3],
	              /* drawData */_createEmptyDrawData(/* () */0),
	              /* imguiFuncData */record[/* imguiFuncData */5],
	              /* ioData */ioData,
	              /* layoutData */record[/* layoutData */7],
	              /* extendData */record[/* extendData */8]
	            ], data);
	}

	function _unbindVAO(gl) {
	  var match = gl.getExtension("OES_vertex_array_object");
	  if (match == null) {
	    return /* () */0;
	  } else {
	    match.bindVertexArrayOES(null);
	    return /* () */0;
	  }
	}

	function _backupGlState(gl, record) {
	  var init = unsafeGetWebglData(record);
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData *//* record */[
	            /* program */init[/* program */0],
	            /* positionBuffer */init[/* positionBuffer */1],
	            /* colorBuffer */init[/* colorBuffer */2],
	            /* texCoordBuffer */init[/* texCoordBuffer */3],
	            /* indexBuffer */init[/* indexBuffer */4],
	            /* fontTexture */init[/* fontTexture */5],
	            /* aPositonLocation */init[/* aPositonLocation */6],
	            /* aColorLocation */init[/* aColorLocation */7],
	            /* aTexCoordLocation */init[/* aTexCoordLocation */8],
	            /* lastWebglData *//* record */[
	              /* lastProgram */nullable_to_opt(gl.getParameter(gl.CURRENT_PROGRAM)),
	              /* lastElementArrayBuffer */gl.getParameter(gl.ELEMENT_ARRAY_BUFFER_BINDING),
	              /* lastArrayBuffer */gl.getParameter(gl.ARRAY_BUFFER_BINDING),
	              /* lastTexture */nullable_to_opt(gl.getParameter(gl.TEXTURE_BINDING_2D)),
	              /* lastIsEnableDepthTest */gl.isEnabled(gl.DEPTH_TEST),
	              /* lastIsEnableBlend */gl.isEnabled(gl.BLEND)
	            ]
	          ],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData */record[/* layoutData */7],
	          /* extendData */record[/* extendData */8]
	        ];
	}

	function _setGlState(gl) {
	  gl.disable(gl.DEPTH_TEST);
	  gl.enable(gl.BLEND);
	  gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
	  return /* () */0;
	}

	function _draw(gl, drawElementsDataArr, record) {
	  var match = unsafeGetWebglData(record);
	  var fontTexture = match[/* fontTexture */5];
	  forEach((function (param) {
	          var texture = param[/* drawType */0] ? unsafeGet(param[/* customTexture */1]) : fontTexture;
	          gl.bindTexture(gl.TEXTURE_2D, texture);
	          gl.drawElements(gl.TRIANGLES, param[/* count */2], gl.UNSIGNED_SHORT, param[/* countOffset */3]);
	          return /* () */0;
	        }), drawElementsDataArr);
	  return record;
	}

	function _restoreGlState(gl, record) {
	  var match = unsafeGet(unsafeGetWebglData(record)[/* lastWebglData */9]);
	  var lastTexture = match[/* lastTexture */3];
	  var lastProgram = match[/* lastProgram */0];
	  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, match[/* lastElementArrayBuffer */1]);
	  gl.bindBuffer(gl.ARRAY_BUFFER, match[/* lastArrayBuffer */2]);
	  if (lastProgram !== undefined) {
	    gl.useProgram(valFromOption(lastProgram));
	  }
	  if (lastTexture !== undefined) {
	    gl.bindTexture(gl.TEXTURE_2D, valFromOption(lastTexture));
	  }
	  if (match[/* lastIsEnableDepthTest */4]) {
	    gl.enable(gl.DEPTH_TEST);
	  } else {
	    gl.disable(gl.DEPTH_TEST);
	  }
	  if (match[/* lastIsEnableBlend */5]) {
	    gl.enable(gl.BLEND);
	  } else {
	    gl.disable(gl.BLEND);
	  }
	  return record;
	}

	function _buildGroupedDrawDataArr(record) {
	  var fontTextureDrawData = getFontTextureDrawData(record);
	  var customTextureDrawDataMap = getCustomTextureDrawDataMap(record);
	  var match = reduceOneParam((function (param, drawData) {
	          var baseIndex = getBaseIndex(param[0]) + param[1] | 0;
	          return /* tuple */[
	                  drawData[/* verticeArr */2],
	                  baseIndex,
	                  push(/* record */[
	                        /* drawType */drawData[/* drawType */0],
	                        /* customTexture */drawData[/* customTexture */1],
	                        /* verticeArr */drawData[/* verticeArr */2],
	                        /* colorArr */drawData[/* colorArr */3],
	                        /* texCoordArr */drawData[/* texCoordArr */4],
	                        /* indexArr */drawData[/* indexArr */5].map((function (index) {
	                                return index + baseIndex | 0;
	                              }))
	                      ], param[2])
	                ];
	        }), /* tuple */[
	        fontTextureDrawData[/* verticeArr */2],
	        0,
	        /* array */[]
	      ], getValidValues(customTextureDrawDataMap));
	  return /* tuple */[
	          record,
	          concatArrays(/* array */[
	                /* array */[fontTextureDrawData],
	                match[2]
	              ])
	        ];
	}

	function _finish(gl, param, data) {
	  var record = _1(param[0], data);
	  _unbindVAO(gl);
	  var record$1 = _backupGlState(gl, record);
	  var match = _buildGroupedDrawDataArr(record$1);
	  var match$1 = bufferAllData(gl, match[1], match[0]);
	  var record$2 = match$1[0];
	  var match$2 = unsafeGetWebglData(record$2);
	  gl.useProgram(match$2[/* program */0]);
	  _setGlState(gl);
	  return _2(param[1], _restoreGlState(gl, _draw(gl, match$1[1], record$2)), data);
	}

	function getCustomData$2(param) {
	  return param[/* imguiFuncData */5][/* customDataForIMGUIFunc */1];
	}

	function _clearData(record) {
	  return /* record */[
	          /* setting */record[/* setting */0],
	          /* assetData */record[/* assetData */1],
	          /* fontData */record[/* fontData */2],
	          /* webglData */record[/* webglData */3],
	          /* drawData */record[/* drawData */4],
	          /* imguiFuncData */record[/* imguiFuncData */5],
	          /* ioData */record[/* ioData */6],
	          /* layoutData : record */[/* groupData : record */[
	              /* positionArr : array */[],
	              /* index */0
	            ]],
	          /* extendData */record[/* extendData */8]
	        ];
	}

	function getIMGUIFunc$2(param) {
	  return param[/* imguiFuncData */5][/* imguiFunc */0];
	}

	function setIMGUIFunc$2(customData, func, record) {
	  var init = record[/* imguiFuncData */5];
	  return _clearData(/* record */[
	              /* setting */record[/* setting */0],
	              /* assetData */record[/* assetData */1],
	              /* fontData */record[/* fontData */2],
	              /* webglData */record[/* webglData */3],
	              /* drawData */record[/* drawData */4],
	              /* imguiFuncData : record */[
	                /* imguiFunc */some$1(func),
	                /* customDataForIMGUIFunc */some$1(customData),
	                /* apiJsObj */init[/* apiJsObj */2]
	              ],
	              /* ioData */record[/* ioData */6],
	              /* layoutData */record[/* layoutData */7],
	              /* extendData */record[/* extendData */8]
	            ]);
	}

	function getAPIJsObj(param) {
	  return param[/* imguiFuncData */5][/* apiJsObj */2];
	}

	function _exec(apiJsObj, getRecordFunc, data) {
	  var record = _1(getRecordFunc, data);
	  var match = getIMGUIFunc$2(record);
	  if (match !== undefined) {
	    return valFromOption(match)(unsafeGet(record[/* imguiFuncData */5][/* customDataForIMGUIFunc */1]), apiJsObj, data);
	  } else {
	    return data;
	  }
	}

	function render$2(gl, ioData, apiJsObj, param, data) {
	  var setRecordFunc = param[1];
	  var getRecordFunc = param[0];
	  var record = _1(getRecordFunc, data);
	  var match = !isLoadAsset(record);
	  if (match) {
	    return data;
	  } else {
	    return _finish(gl, /* tuple */[
	                getRecordFunc,
	                setRecordFunc
	              ], _exec(apiJsObj, getRecordFunc, _prepare(ioData, /* tuple */[
	                        getRecordFunc,
	                        setRecordFunc
	                      ], data)));
	  }
	}

	function createRecord$2(param) {
	  return /* record */[
	          /* setting : record */[
	            /* textColor : array */[
	              1,
	              1,
	              1
	            ],
	            /* fontTexUvForWhite : array */[
	              0,
	              0
	            ]
	          ],
	          /* assetData : record */[
	            /* fntId */"fnt",
	            /* bitmapId */"bitmap",
	            /* fntDataMap */createEmpty$1(/* () */0),
	            /* bitmapMap */createEmpty$1(/* () */0),
	            /* customImageArr : array */[],
	            /* customTextureMap */createEmpty$1(/* () */0)
	          ],
	          /* fontData */undefined,
	          /* webglData */undefined,
	          /* drawData */_createEmptyDrawData(/* () */0),
	          /* imguiFuncData : record */[
	            /* imguiFunc */undefined,
	            /* customDataForIMGUIFunc */undefined,
	            /* apiJsObj */{
	              label: label$1,
	              image: image$1,
	              box: box$1,
	              beginGroup: beginGroup$1,
	              endGroup: endGroup$1,
	              unsafeGetCustomControl: unsafeGetCustomControl
	            }
	          ],
	          /* ioData : record */[
	            /* pointUp */false,
	            /* pointDown */false,
	            /* pointPosition : tuple */[
	              0,
	              0
	            ],
	            /* pointMovementDelta : tuple */[
	              0,
	              0
	            ]
	          ],
	          /* layoutData : record */[/* groupData : record */[
	              /* positionArr : array */[],
	              /* index */0
	            ]],
	          /* extendData : record */[
	            /* customControlData : record */[
	              /* apiJsObj */createAPIJsObj(/* () */0),
	              /* funcMap */createEmpty$5(/* () */0)
	            ],
	            /* skinData : record */[/* allSkinDataMap */createEmpty$5(/* () */0)]
	          ]
	        ];
	}


	/* OptionService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	function _getRecord(record) {
	  return record;
	}

	function _setRecord(record, data) {
	  return record;
	}

	function render$1(gl, ioDataJsObj, record) {
	  return render$2(gl, /* record */[
	              /* pointUp */ioDataJsObj.pointUp,
	              /* pointDown */ioDataJsObj.pointDown,
	              /* pointPosition */ioDataJsObj.pointPosition,
	              /* pointMovementDelta */ioDataJsObj.pointMovementDelta
	            ], getAPIJsObj(record), /* tuple */[
	              _getRecord,
	              _setRecord
	            ], record);
	}

	function getSetting$2(record) {
	  var match = getSetting$1(record);
	  return {
	          textColor: match[/* textColor */0],
	          fontTexUvForWhite: match[/* fontTexUvForWhite */1]
	        };
	}

	function setSetting$2(settingJsObj, record) {
	  return setSetting$1(/* record */[
	              /* textColor */settingJsObj.textColor,
	              /* fontTexUvForWhite */settingJsObj.fontTexUvForWhite
	            ], record);
	}

	var sendUniformProjectionMatData$1 = sendUniformProjectionMatData$2;

	var createRecord$1 = createRecord$2;

	var init$1 = init$2;

	var getCustomData$1 = getCustomData$2;

	var getIMGUIFunc$1 = getIMGUIFunc$2;

	var setIMGUIFunc$1 = setIMGUIFunc$2;


	/* ManageIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	var beginGroup$2 = beginGroup$1;

	var endGroup$2 = endGroup$1;


	/* No side effect */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	var label$2 = label$1;

	var image$2 = image$1;

	var box$2 = box$1;


	/* FixedLayoutControlIMGUIService-WonderImgui Not a pure module */

	// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE

	var setCustomImageArr = setCustomImageArr$2;

	var getCustomImageArr = getCustomImageArr$2;

	var setBitmap = setBitmap$2;

	var getBitmap = getBitmap$2;

	var setFntData = setFntData$2;

	var getFntData = getFntData$2;

	var addDefaultSkinData = addDefaultSkinData$1;

	var clearAllSkins = clearAllSkins$1;

	var createSkinData = createSkinData$1;

	var removeSkinData = removeSkinData$1;

	var addSkinData = addSkinData$1;

	var removeSingleCustomStyleData = removeSingleCustomStyleData$1;

	var addSingleCustomStyleData = addSingleCustomStyleData$1;

	var removeCustomStyleData = removeCustomStyleData$1;

	var addCustomStyleData = addCustomStyleData$1;

	var createSingleCustomStyleData = createSingleCustomStyleData$1;

	var createAllCustomStyleData = createAllCustomStyleData$1;

	var registerCustomControl = registerCustomControl$1;

	var box = box$2;

	var image = image$2;

	var label = label$2;

	var endGroup = endGroup$2;

	var beginGroup = beginGroup$2;

	var buildIOData = buildIOData$1;

	var addFont = addFont$1;

	var load = load$1;

	var sendUniformProjectionMatData = sendUniformProjectionMatData$1;

	var setIMGUIFunc = setIMGUIFunc$1;

	var getIMGUIFunc = getIMGUIFunc$1;

	var getCustomData = getCustomData$1;

	var setSetting = setSetting$2;

	var getSetting = getSetting$2;

	var render = render$1;

	var init = init$1;

	var createRecord = createRecord$1;


	/* IOIMGUIAPI-WonderImgui Not a pure module */

	exports.setCustomImageArr = setCustomImageArr;
	exports.getCustomImageArr = getCustomImageArr;
	exports.setBitmap = setBitmap;
	exports.getBitmap = getBitmap;
	exports.setFntData = setFntData;
	exports.getFntData = getFntData;
	exports.addDefaultSkinData = addDefaultSkinData;
	exports.clearAllSkins = clearAllSkins;
	exports.createSkinData = createSkinData;
	exports.removeSkinData = removeSkinData;
	exports.addSkinData = addSkinData;
	exports.removeSingleCustomStyleData = removeSingleCustomStyleData;
	exports.addSingleCustomStyleData = addSingleCustomStyleData;
	exports.removeCustomStyleData = removeCustomStyleData;
	exports.addCustomStyleData = addCustomStyleData;
	exports.createSingleCustomStyleData = createSingleCustomStyleData;
	exports.createAllCustomStyleData = createAllCustomStyleData;
	exports.registerCustomControl = registerCustomControl;
	exports.box = box;
	exports.image = image;
	exports.label = label;
	exports.endGroup = endGroup;
	exports.beginGroup = beginGroup;
	exports.buildIOData = buildIOData;
	exports.addFont = addFont;
	exports.load = load;
	exports.sendUniformProjectionMatData = sendUniformProjectionMatData;
	exports.setIMGUIFunc = setIMGUIFunc;
	exports.getIMGUIFunc = getIMGUIFunc;
	exports.getCustomData = getCustomData;
	exports.setSetting = setSetting;
	exports.getSetting = getSetting;
	exports.render = render;
	exports.init = init;
	exports.createRecord = createRecord;

	Object.defineProperty(exports, '__esModule', { value: true });

})));
